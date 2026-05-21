#ifndef _IF_ENTROPYMAP_HPP
#define _IF_ENTROPYMAP_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
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

#include "fmt/base.h"
#include "fmt/format.h"

namespace IF_EntropyMap
{

class Instruction
{
public:
    using idx_t = uint32_t;
    using succs_t = std::unordered_set<const Instruction*>;

    friend class Map;

private:
    idx_t idx;
    const unsigned int opcode;
    const unsigned int ret_ty_bit_sz;
    std::set<llvm::Value*> insts_in;

    const Instruction* succ_natural = nullptr;
    succs_t succs_instr;
    std::set<std::string> succs_extern;

    double retained_entropy;
    bool trivial = false;


public:

    const bool llvm_no_uses;
    const bool llvm_is_terminator;

    /* Constructors ***********************************************************/

    Instruction(uint32_t, const llvm::Instruction&);

    /* Getters ****************************************************************/

    idx_t get_idx(void) const { return this->idx; };

    unsigned int get_opcode(void) const { return this->opcode; };

    auto get_reg_uses(void) const -> decltype(this->insts_in)
    {
        return this->insts_in;
    };

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

    void add_successor(const Instruction*);

    void add_external_succ(std::string);

    /* Printers ***************************************************************/

    const std::string to_str(void) const;

    const std::string to_str_simple(void) const;

    /* Operators **************************************************************/

    bool operator==(const Instruction& o) const
    {
        return this->idx == o.get_idx();
    };
};

using insts_t = std::vector<const IF_EntropyMap::Instruction*>;
using insts_llvm_mapping_t = std::unordered_map<const llvm::Instruction*,
    const IF_EntropyMap::Instruction*>;

class Function
{
private:
    const std::string name;
    const std::string demangled_name;
    std::vector<std::unique_ptr<Instruction>> instrs;

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

    /* Getters ****************************************************************/

    const std::string get_name(void) const { return this->name; };

    const std::string get_demangled_name(void) const
    {
        return this->demangled_name;
    };

    const std::string get_representing_name(void) const;

    const Instruction* get_first_instr() const;

    auto get_instrs(void) const -> const decltype(this->instrs)&
    {
        return this->instrs;
    };

    /* Setters ****************************************************************/

    void insert(std::unique_ptr<Instruction> _instr)
    {
        this->instrs.push_back(std::move(_instr));
    };

    /* Printers ***************************************************************/

    const std::string to_str(void) const;
};

class Map
{
private:
    std::vector<std::unique_ptr<Function>> funcs;
    std::set<std::string> external_funcs;
    bool verbose = false;
    uint32_t instr_count;

public:
    /* Constructors ***********************************************************/

    Map(const llvm::Module& _module) { this->funcs.reserve(_module.size()); };

    /* Getters ****************************************************************/

    const Instruction* get_first_instr(void) const;

    inline auto get_funcs(void) const -> const decltype(funcs)&
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

    void insert(std::unique_ptr<Function> em_fn)
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

    std::tuple<size_t, size_t, size_t> compute_cyclomatic_complexity(
        void) const;

    /* Printers ***************************************************************/

    const std::string to_str(void) const;
    void print(void) const;
};

class UseMap
{
    struct Node
    {
        const IF_EntropyMap::Instruction* em_inst;
        std::vector<const IF_EntropyMap::UseMap::Node*> preds;

        Node(const IF_EntropyMap::Instruction*);

        void add_pred(const IF_EntropyMap::UseMap::Node*);
        double get_unc_coef(void) const;
        auto get_idx(void) const -> IF_EntropyMap::Instruction::idx_t;

        std::string to_str_path(uint32_t) const;
    };

    struct UC_Path
    {
        const IF_EntropyMap::UseMap::Node* last_node;
        std::string node_path;
        double unc_coef;

        UC_Path(const IF_EntropyMap::UseMap::Node*);

        std::string to_str(void) const;
    };

    struct Path
    {
        using uc_paths_t = std::vector<UC_Path>;
        std::vector<const IF_EntropyMap::UseMap::Node*> root_nodes;

        Path(void);

        uc_paths_t compute_unc_coef(void) const;
        std::string to_str(void) const;
        // void combine(
        // IF_EntropyMap::UseMap::Path*, const IF_EntropyMap::Instruction*);
        // void add_pred(const IF_EntropyMap::Instruction*,
        // const IF_EntropyMap::Instruction*);
    };

private:
    IF_EntropyMap::UseMap::Path* use_path;

public:
    UseMap(const insts_t&, const insts_llvm_mapping_t&);
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
