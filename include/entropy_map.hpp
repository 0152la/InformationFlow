#ifndef _IF_ENTROPYMAP_HPP
#define _IF_ENTROPYMAP_HPP

// #include <format>
#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#pragma clang diagnostic pop

namespace IF_EntropyMap
{

class Instruction
{
public:
    using idx_t = uint32_t;
    using succs_t = std::unordered_set<const Instruction*>;

private:
    idx_t idx;
    unsigned int opcode;

    const Instruction* succ_natural = nullptr;
    succs_t succs_instr;
    std::set<std::string> succs_extern;

    double retained_entropy;
    bool trivial = false;

    bool compressed = false;
    size_t compress_count = 0;

public:
    /* Constructors ***********************************************************/

    Instruction(uint32_t _idx, const llvm::Instruction& _instr) :
        idx(_idx),
        opcode(_instr.getOpcode()) { };

    /* Getters ****************************************************************/

    idx_t get_idx(void) const { return this->idx; };

    unsigned int get_opcode(void) const { return this->opcode; };

    double get_retained_entropy(void) const { return this->retained_entropy; };

    const succs_t& get_succs_inst(void) const { return this->succs_instr; };

    size_t get_succs_count(void) const { return this->succs_instr.size(); };

    auto get_external_succs(void) const -> const decltype(this->succs_extern)&
    {
        return this->succs_extern;
    };

    const Instruction* get_natural_successor(void) const
    {
        return this->succ_natural;
    };

    const succs_t& get_all_successors(void) const { return this->succs_instr; };

    bool is_trivial(void) const { return this->trivial; };

    /* Setters ****************************************************************/

    void set_retained_entropy(double _entropy)
    {
        this->retained_entropy = _entropy;
        if (_entropy == 1.0)
        {
            this->trivial = true;
        }
    };

    void set_natural_successor(const Instruction* succ)
    {
        this->succ_natural = succ;
    };

    // TODO external succs?
    inline void clear_succs(void) { this->succs_instr.clear(); };

    void add_successor(const Instruction*);

    void add_external_succ(std::string);

    inline void set_compressed_count(size_t to_set)
    {
        this->compressed = true;
        this->compress_count = to_set;
    };

    /* Printers ***************************************************************/

    const std::string to_str(void) const;

    const std::string to_str_simple(void) const;

    /* Operators **************************************************************/

    bool operator==(const Instruction& o) const
    {
        return this->idx == o.get_idx();
    };
};

class Function
{
public:
    using insts_t = std::vector<Instruction*>;

private:
    const std::string name;
    const std::string demangled_name;
    insts_t instrs;

    const std::string set_demangled_name(const llvm::Function&);

public:
    /* Constructors ***********************************************************/

    Function() = delete;

    Function(const llvm::Function& _fn) :
        name(_fn.getName().str()),
        demangled_name(Function::set_demangled_name(_fn))
    {
        this->instrs.reserve(_fn.getInstructionCount());
    };

    ~Function();

    /* Getters ****************************************************************/

    const std::string get_name(void) const { return this->name; };

    const std::string get_demangled_name(void) const
    {
        return this->demangled_name;
    };

    const std::string get_representing_name(void) const;

    const Instruction* get_first_instr() const;

    const insts_t& get_instrs(void) const { return this->instrs; };

    inline size_t get_inst_count(void) const { return this->instrs.size(); };

    inline size_t get_nontrivial_inst_count(void) const
    {
        return std::count_if(this->instrs.begin(), this->instrs.end(),
            [](const auto& inst) { return !inst->is_trivial(); });
    };

    /* Setters ****************************************************************/

    inline void set_insts(insts_t new_insts)
    {
        this->instrs = new_insts;
    };

    void insert(insts_t::value_type _instr)
    {
        this->instrs.push_back(_instr);
    };

    /* Printers ***************************************************************/

    const std::string to_str(void) const;
};

class Map
{
    public:
        using funcs_t = std::vector<Function*>;
private:
    funcs_t funcs;
    std::set<std::string> external_funcs;
    bool verbose = false;
    uint32_t instr_count;

    bool compressed = false;
    uint32_t compressed_instr_count;

public:
    /* Constructors ***********************************************************/

    Map(const llvm::Module& _module) { this->funcs.reserve(_module.size()); };
    ~Map();

    /* Getters ****************************************************************/

    const Instruction* get_first_instr(void) const;

    inline const funcs_t& get_funcs(void) const
    {
        return this->funcs;
    };

    inline auto get_external_funcs(void) const -> const
        decltype(external_funcs)&
    {
        return this->external_funcs;
    };

    inline auto get_instruction_count(void) const -> decltype(instr_count)
    {
        return this->instr_count;
    }

    auto get_nontrivial_instruction_count(void) const -> decltype(instr_count);

    /* Setters ****************************************************************/

    void insert(funcs_t::value_type em_fn)
    {
        this->funcs.push_back(std::move(em_fn));
    };

    void insert_external_func(std::string);

    inline void set_verbose(bool _verbose) { this->verbose = _verbose; };

    inline void set_instruction_count(uint32_t instr_count)
    {
        this->instr_count = instr_count;
    };

    /* Others *****************************************************************/

    void compress_map(void);

    std::tuple<size_t, size_t, size_t> compute_cyclomatic_complexity(
        void) const;

    /* Printers ***************************************************************/

    const std::string to_str(void) const;
    const std::string to_str_summary(void) const;
    void print(void) const;
};

}; // namespace IF_EntropyMap

namespace std
{
template <> struct hash<IF_EntropyMap::Instruction>
{
    size_t operator()(IF_EntropyMap::Instruction const& if_em_i) const noexcept
    {
        return std::hash<int>()(if_em_i.get_idx());
    }
};
}

#endif // _IF_ENTROPYMAP_HPP
