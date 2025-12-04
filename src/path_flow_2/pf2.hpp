#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using val_t = uint64_t;
using idx_t = uint16_t;

class ObsNode
{
public:
    idx_t idx;
    bool is_cond;

    ObsNode(idx_t _idx, bool _cond) :
        idx(_idx),
        is_cond(_cond) { };
    ObsNode(idx_t _idx) :
        ObsNode(_idx, false) { };

    std::string to_str(void) const;
};

class ObsOp
{
public:
    val_t op_in;
    val_t op_out;
    val_t prog_in;
    idx_t op_idx;

    ObsOp(val_t _oin, val_t _oout, val_t _pin, idx_t _oidx) :
        op_in(_oin),
        op_out(_oout),
        prog_in(_pin),
        op_idx(_oidx) { };

    std::string to_str(void) const;
};

struct EntropySupport
{
    std::vector<size_t> instances;
    size_t total_instances;

    EntropySupport(const std::vector<val_t>& _inst, size_t _tot) :
        instances(_inst),
        total_instances(_tot) { };
};

class ObsProg
{
private:
    std::vector<ObsOp*> obs;
    std::vector<ObsNode*> nodes;

public:
    ~ObsProg();

    auto get_obs(void) const -> const decltype(this->obs)&
    {
        return this->obs;
    };

    val_t add_obs(val_t, val_t, val_t, idx_t, bool = false);

    ObsNode* get_node(idx_t idx)
    {
        decltype(this->nodes)::iterator n_it = std::find_if(this->nodes.begin(),
            this->nodes.end(), [&idx](ObsNode* n) { return n->idx == idx; });
        if (n_it == this->nodes.end())
        {
            throw std::runtime_error(
                "Couldn't find node idx " + std::to_string(idx));
        }
        return *n_it;
    }

    void compute_path_entropy(std::vector<idx_t>&&);
    std::string to_str(bool) const;
};

struct ObsPath
{
    std::vector<ObsNode*> obs_path;
    std::vector<val_t> prog_in_vals;
    std::vector<ObsOp*> obs;

    ObsPath(ObsProg*, const std::vector<idx_t>&);

    double compute_entropy(void) const;

    std::string to_str(bool = false) const;
};

uint64_t
me_hash(uint64_t, uint64_t);
double compute_entropy(std::vector<val_t>);
EntropySupport
get_instances(const ObsOp*, int);
