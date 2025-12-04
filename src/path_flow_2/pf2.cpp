#include "pf2.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

constexpr uint8_t bit_sz = 6;
constexpr val_t val_max = 1 << bit_sz;

/* Helper *********************************************************************/

uint64_t
me_hash(uint32_t arg_count, ...)
{
    uint64_t h = 0;
    const auto up_h
        = [&h](uint64_t v) { h ^= (v + 0x9e3779b9 + (h << 6) + (h >> 2)); };

    std::va_list rest;
    va_start(rest, arg_count);
    for (decltype(arg_count) i = 0; i < arg_count; ++i)
    {
        up_h(va_arg(rest, uint64_t));
    }
    va_end(rest);

    return h;
}

double
compute_entropy_from_support(const EntropySupport& es)
{
    double sum = 0;
    double prob;
    for (size_t i = 0; i < es.instances.size(); ++i)
    {
        prob = 1.0 * es.instances[i] / es.total_instances;
        sum += prob * std::log2(prob);
    }
    return -sum;
}

EntropySupport
get_instances(const std::vector<ObsOp*> obs, int in_or_out_todo)
{
    std::unordered_map<val_t, size_t> instances;

    for (const ObsOp* o : obs)
    {
        val_t curr_val = in_or_out_todo == 0 ? o->op_in : o->op_out;
        if (instances.find(curr_val) == instances.end())
        {
            instances.insert(std::make_pair(curr_val, 0));
        }
        instances.at(curr_val) += 1;
    }

    std::vector<size_t> i_counts;
    size_t total = 0;
    for (const auto& i : instances)
    {
        i_counts.push_back(i.second);
        total += i.second;
    }
    return EntropySupport(i_counts, total);
}

/* ObsNode*********************************************************************/

std::string
ObsNode::to_str(void) const
{
    std::ostringstream oss;
    oss << "Node -- IDX " << this->idx << " -- COND "
        << (this->is_cond ? "true" : "false") << '\n';
    return oss.str();
}

/* ObsOp **********************************************************************/

std::string
ObsOp::to_str(void) const
{
    std::ostringstream oss;
    oss << "Obs -- PROG_IN " << this->prog_in << " -- VAL_IN " << this->op_in
        << " -- VAL_OUT " << this->op_out << " -- OP_IDX " << this->op_idx
        << '\n';
    return oss.str();
}

/* ObsPath ********************************************************************/

ObsPath::ObsPath(ObsProg* _op, const std::vector<idx_t>& _idx_path)
{
    std::vector<idx_t>::const_iterator curr_idx = _idx_path.begin();
    this->obs_path.push_back(_op->get_node(*curr_idx));
    for (const auto& op_obs : _op->get_obs())
    {
        if (op_obs->op_idx == *curr_idx)
        {
            this->obs.push_back(op_obs);
            this->prog_in_vals.push_back(op_obs->prog_in);
        }
    }

    curr_idx = std::next(curr_idx);
    std::vector<val_t> curr_op_prog_ins;
    std::vector<val_t> val_in_intersect;
    while (curr_idx != _idx_path.end())
    {
        this->obs_path.push_back(_op->get_node(*curr_idx));

        std::sort(this->prog_in_vals.begin(), this->prog_in_vals.end());
        val_in_intersect.clear();
        curr_op_prog_ins.clear();
        for (const auto& op_obs : _op->get_obs())
        {
            if (op_obs->op_idx == *curr_idx
                && std::find(this->prog_in_vals.begin(),
                       this->prog_in_vals.end(), op_obs->prog_in)
                    != this->prog_in_vals.end())
            {
                this->obs.push_back(op_obs);
                curr_op_prog_ins.push_back(op_obs->prog_in);
            }
        }
        std::sort(curr_op_prog_ins.begin(), curr_op_prog_ins.end());
        std::set_intersection(curr_op_prog_ins.begin(), curr_op_prog_ins.end(),
            this->prog_in_vals.begin(), this->prog_in_vals.end(),
            std::back_inserter(val_in_intersect));
        this->prog_in_vals = val_in_intersect;
        curr_idx = std::next(curr_idx);
    }
}

double
ObsPath::compute_entropy(void) const
{
    double entropy = 1.0;

    decltype(this->obs_path)::const_reverse_iterator n_it
        = this->obs_path.crbegin();
    std::set<val_t> in_scope_vals;
    std::vector<ObsOp*> node_obs;
    std::vector<std::pair<double, double>> entropies;

    // We iterate over nodes in reverse order, so that for a conditional node,
    // we can see the proportion of passed entropy by the proportion of values
    // which are present in a successor node (i.e. the one that was processed
    // before the conditional node
    while (n_it != this->obs_path.crend())
    {
        // std::cout << "NODE " << (*n_it)->idx << " -- ";

        node_obs.clear();
        std::copy_if(this->obs.begin(), this->obs.end(),
            std::back_inserter(node_obs),
            [&n_it](const ObsOp* o) { return (*n_it)->idx == o->op_idx; });

        if ((*n_it)->is_cond)
        {
            double proportion = 1.0 * in_scope_vals.size() / node_obs.size();
            entropies.push_back(std::make_pair(proportion, 1.0));
            entropy *= proportion;
            // std::cout << "PROP " << proportion;
        }
        else
        {
            EntropySupport es_in = get_instances(node_obs, 0);
            EntropySupport es_out = get_instances(node_obs, 1);

            double h_in = compute_entropy_from_support(es_in);
            double h_out = compute_entropy_from_support(es_out);

            if (h_in == 0.0 || h_out == 0.0)
            {
                entropy *= 0.0;
            }

            entropies.push_back(std::make_pair(h_out, h_in));

            // std::cout << "H IN " << h_in << " -- H_OUT " << h_out
            //<< " -- U COEF " << h_out / h_in;
            entropy *= h_out / h_in;
        }

        // std::cout << std::endl;

        std::transform(node_obs.begin(), node_obs.end(),
            std::inserter(in_scope_vals, in_scope_vals.end()),
            [](const ObsOp* o) { return o->prog_in; });

        n_it = std::next(n_it);
    }

    for (const auto& [o, i] : entropies)
    {
        std::cout << "OUT " << o << " -- IN " << i << '\n';
    }

    std::cout << this->to_str(false);
    std::cout << " * Computed u_coef == " << entropy << '\n';
    std::cout << " * Validate list == "
              << std::accumulate(entropies.begin(), entropies.end(), 1.0,
                     [](double uc, decltype(entropies)::value_type hs)
                     { return uc * hs.first / hs.second; })
              << '\n';

    // TODO ignore conditional nodes?
    std::cout << " * Full path u_coef == "
              << entropies.front().first / entropies.back().second << '\n';

    return entropy;
}

std::string
ObsPath::to_str(bool verb) const
{
    std::ostringstream oss;

    std::cout << "=== PATH "
              << std::accumulate(std::next(this->obs_path.begin()),
                     this->obs_path.end(),
                     std::to_string(this->obs_path.front()->idx),
                     [](std::string s, const ObsNode* n)
                     { return s + ',' + std::to_string(n->idx); })
              << std::endl;

    std::cout << "-- INPUTS "
              << std::accumulate(std::next(this->prog_in_vals.begin()),
                     this->prog_in_vals.end(),
                     std::to_string(this->prog_in_vals.front()),
                     [](std::string s, val_t v)
                     { return s + ',' + std::to_string(v); })
              << std::endl;

    if (verb)
    {
        for (const auto& o : this->obs)
        {
            oss << o->to_str();
        }
    }
    return oss.str();
}

/* ObsProg ********************************************************************/

ObsProg::~ObsProg()
{
    for (const auto& o : this->obs)
    {
        delete o;
    }
    for (const auto& n : this->nodes)
    {
        delete n;
    }
}

val_t
ObsProg::add_obs(
    val_t _out, val_t _in, val_t prog_in_val, idx_t idx, bool is_cond)
{
    if (std::find_if(this->nodes.begin(), this->nodes.end(),
            [&idx](const ObsNode* n) { return n->idx == idx; })
        == this->nodes.end())
    {
        this->nodes.push_back(new ObsNode(idx, is_cond));
    }

    ObsOp* new_obs = new ObsOp(_in, _out, prog_in_val, idx);
    this->obs.push_back(new_obs);
    return _out;
}

void
ObsProg::compute_path_entropy(std::vector<idx_t>&& idx_path)
{
    ObsPath op(this, idx_path);
    op.compute_entropy();
}

std::string
ObsProg::to_str(bool verbose) const
{
    std::ostringstream oss;
    if (verbose)
    {
        std::cout << "== Nodes\n";
        for (const auto& n : this->nodes)
        {
            oss << n->to_str();
        }

        std::cout << "== Observations\n";
        for (const auto& o : this->obs)
        {
            oss << o->to_str();
        }
    }
    return oss.str();
}

/* Test functions *************************************************************/

static val_t
one_iter_log(val_t v_in, ObsProg& o)
{
    val_t t = 0;
    bool branch = o.add_obs(v_in % 4 == 0, v_in, v_in, 0, true);
    if (branch)
    {
        t = o.add_obs(v_in / 4, v_in, v_in, 1);
    }
    else
    {
        t = o.add_obs(5, t, v_in, 2);
    }
    val_t ret;
    ret = o.add_obs(v_in + t, me_hash(2, v_in, t), v_in, 3);
    ret = o.add_obs(ret % val_max, me_hash(2, ret, val_max), v_in, 4);
    return ret;
}

static val_t
one_iter(val_t v_in)
{
    val_t t;
    if (v_in % 4 == 0)
    {
        t = v_in / 4;
    }
    else
    {
        t = 5;
    }
    return (v_in + t) % val_max;
}

/* Main ***********************************************************************/

int
main()
{
    ObsProg o;
    for (size_t i = 0; i < val_max; ++i)
    {
        one_iter_log(i, o);
    }

    o.compute_path_entropy(std::vector<idx_t> { 0, 1, 3, 4 });
    o.compute_path_entropy(std::vector<idx_t> { 0, 2, 3, 4 });
    return 0;
}
