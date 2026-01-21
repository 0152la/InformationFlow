#include "runner.hpp"
#include <unordered_map>

/*******************************************************************************
 * DefInfo
 ******************************************************************************/

DefInfo::DefInfo(const std::string& line)
{
    // opcode, cmp_opcode, name_extra, fn_name(ret_ty)[params_ty]
    std::regex re("(\\d+),(\\d*),(\\w*),(\\w+)\\((\\w+)\\)\\[(.+)\\]");
    std::smatch sm;

    if (!std::regex_match(line, sm, re))
    {
        std::cout << "Error matching line " << line << "!\n";
        std::exit(1);
    }

    this->llvm_opcode = std::stoul(sm[1].str());
    this->cmp_opcode = sm[2].str().empty() ? -1 : std::stoul(sm[2].str());
    this->extra_fn_name = sm[3].str();

    this->llvm_fn_name = sm[4].str();
    this->ret_ty = def_ty_enum::from_str(sm[5].str());
    this->ret_str = sm[5].str();

    this->params_str = sm[6].str();
    std::istringstream param_iss(sm[6].str());
    std::string p;
    while (std::getline(param_iss, p, ','))
    {
        this->params_ty.emplace_back(def_ty_enum::from_str(p));
    }
}

std::string
DefInfo::get_fn_name(void) const
{
    std::string name = this->llvm_fn_name;

    if (!this->extra_fn_name.empty())
    {
        name += DefInfo::extra_delim + this->extra_fn_name;
    }

    return name;
}

bool
DefInfo::check_div(void) const
{
    using namespace std::string_view_literals;

    constexpr std::array div_names { "udiv"sv, "sdiv"sv, "urem"sv, "srem"sv };
    const auto name_sv = std::string_view { this->llvm_fn_name };
    if (std::any_of(div_names.begin(), div_names.end(),
            [&name_sv](const std::string_view& s)
            { return name_sv.find(s) != std::string::npos; }))
    {
        return true;
    }
    return false;
}

bool
DefInfo::check_fop(void) const
{
    using namespace std::string_view_literals;

    constexpr std::array fop_names { "fadd"sv, "fmul"sv, "fsub"sv, "fdiv"sv,
        "frem"sv, "fcmp"sv, "fpto"sv };
    const auto name_sv = std::string_view { this->llvm_fn_name };
    if (std::any_of(fop_names.begin(), fop_names.end(),
            [&name_sv](const std::string_view& s)
            { return name_sv.find(s) != std::string::npos; }))
    {
        return true;
    }
    return false;
}

std::string
DefInfo::to_str(void) const
{
    std::ostringstream oss;
    oss << "DefInfo == ";
    oss << "LLVM OPC " << this->llvm_opcode << " - ";
    oss << "CMP OPC " << this->cmp_opcode << " - ";
    oss << "EXTRA " << this->extra_fn_name << " - ";
    oss << "FN " << this->llvm_fn_name << " - ";
    oss << "RET " << this->ret_str << " - ";
    oss << "PARAMS (" << (this->params_str.empty() ? "void" : this->params_str)
        << ") ==";
    return oss.str();
}

/*******************************************************************************
 * Run Info
 ******************************************************************************/

RunInfo::RunInfo(const DefInfo& _di, void* _fn_ptr) :
    di(&_di),
    fn_ptr(_fn_ptr),
    is_div(di->check_div())
{
    if (di->check_fop())
    {
        this->bit_sz_min
            = def_ty_enum::def_ty_fl_bitsizes.at(di->params_ty.front());
        this->bit_sz_max = this->bit_sz_min;
    }
    else
    {
        this->bit_sz_min = def_ty_enum::int_min_bit_sz;
        this->bit_sz_max = def_ty_enum::int_max_bit_sz;
    }
};

/*******************************************************************************
 * Runner
 ******************************************************************************/

Runner::Runner(void)
{
    this->dl_hdl = dlopen((Runner::lib_path).data(), RTLD_NOW);
    if (!this->dl_hdl)
    {
        std::cout << "Error opening library :: " << dlerror() << '\n';
        std::exit(1);
    }

    this->out_fd = std::fopen((Runner::out_path).data(), "w");
    if (this->out_fd == nullptr)
    {
        std::perror("Error opening definitions");
        std::exit(1);
    }
}

Runner::~Runner(void)
{
    dlclose(this->dl_hdl);
    fclose(this->out_fd);
}

const EntropyResult
Runner::run_one(const DefInfo& di) const
{
    void* fn = dlsym(this->dl_hdl, di.get_fn_name().c_str());

    auto ri = RunInfo { di, fn };
    return this->eval_ret(ri);
}

void
Runner::eval_all(void) const
{
    std::string buf;
    std::ifstream def_ifs(this->def_path.data());
    static int i = 0;
    while (std::getline(def_ifs, buf))
    {
        if (buf.starts_with("opcode"))
        {
            continue;
        }

        if (buf.find("fadd") == std::string::npos)
        {
            continue;
        }
        const DefInfo di(buf);
        std::cout << di.to_str() << std::endl;

        const EntropyResult er = this->run_one(di);
        er.print();

        // i += 1; if (i == 2) { std::exit(1); }
        std::exit(1);
    }
}

const EntropyResult
Runner::eval_ret(const RunInfo& ri) const
{
    switch (ri.di->ret_ty)
    {
        case def_ty_enum::I64:
            return this->eval_args<uint64_t>(ri);
        case def_ty_enum::I32:
            return this->eval_args<uint32_t>(ri);
        case def_ty_enum::I16:
            return this->eval_args<uint16_t>(ri);
        case def_ty_enum::F32:
            return this->eval_args<float>(ri);
        case def_ty_enum::F16:
            return this->eval_args<_Float16>(ri);
        default:
            throw std::runtime_error(fmt::format("Unhandled return type {0}!",
                def_ty_enum::to_str(ri.di->ret_ty)));
    }
}
