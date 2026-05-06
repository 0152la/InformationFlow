#include "runner.hpp"
#include "config.hpp"

/*******************************************************************************
 * DefInfoFlags
 ******************************************************************************/

DefInfoFlags::DefInfoFlags(std::string_view _di_fn_name) :
    is_div(this->check_name_within(this->div_insts, _di_fn_name)),
    is_fop(this->check_name_within(this->fop_insts, _di_fn_name)),
    is_overflow(this->check_name_within(this->overflow_insts, _di_fn_name)),
    is_shift(this->check_name_within(this->shift_insts, _di_fn_name))
{
    this->is_overflow |= this->is_shift;
}

DefInfoFlags::DefInfoFlags(const DefInfoFlags& other) :
    is_div(other.is_div),
    is_fop(other.is_fop),
    is_overflow(other.is_overflow),
    is_shift(other.is_shift) { };

/*******************************************************************************
 * DefInfo
 ******************************************************************************/

DefInfo::DefInfo(const std::string& line)
{
    // Line content for def file
    // opcode, cmp_opcode, name_extra, fn_name(ret_ty)[params_ty]
    auto re
        = std::regex { "(\\d+),(\\d*),(\\w*),(\\w+)\\((\\w+)\\)\\[(.+)\\]" };
    auto sm = std::smatch { };

    if (!std::regex_match(line, sm, re))
    {
        fmt::println("Error matching line {}!", line);
        std::exit(1);
    }

    this->llvm_opcode = std::stoul(sm[1].str());
    this->cmp_opcode = sm[2].str().empty() ? -1 : std::stoul(sm[2].str());
    this->extra_fn_name = sm[3].str();

    this->llvm_fn_name = sm[4].str();
    this->ret_ty = def_ty_enum::from_str(sm[5].str());
    this->ret_str = sm[5].str();

    this->params_str = sm[6].str();
    auto param_iss = std::istringstream { sm[6].str() };
    auto p = std::string { };
    while (std::getline(param_iss, p, ','))
    {
        this->params_ty.emplace_back(def_ty_enum::from_str(p));
    }

    this->di_flags = new DefInfoFlags(this->llvm_fn_name);
}

DefInfo::~DefInfo(void) { delete (this->di_flags); }

DefInfo::DefInfo(const DefInfo& other) :
    llvm_opcode(other.llvm_opcode),
    cmp_opcode(other.cmp_opcode),
    extra_fn_name(other.extra_fn_name),
    llvm_fn_name(other.llvm_fn_name),
    ret_ty(other.ret_ty),
    ret_str(other.ret_str),
    params_ty(other.params_ty)
{
    this->di_flags = new DefInfoFlags(*other.di_flags);
}

DefInfo::DefInfo(DefInfo&& other) noexcept :
    llvm_opcode(other.llvm_opcode),
    cmp_opcode(other.cmp_opcode),
    extra_fn_name(other.extra_fn_name),
    llvm_fn_name(other.llvm_fn_name),
    ret_ty(other.ret_ty),
    ret_str(other.ret_str),
    params_ty(other.params_ty)
{
    this->di_flags = std::exchange(other.di_flags, nullptr);
}

DefInfo&
DefInfo::operator=(const DefInfo& other)
{
    return *this = DefInfo(other);
}

DefInfo&
DefInfo::operator=(DefInfo&& other) noexcept
{
    std::swap(this->di_flags, other.di_flags);
    return *this;
}

std::string
DefInfo::get_fn_name(void) const
{
    return this->llvm_fn_name;
}

std::string
DefInfo::get_extra(void) const
{
    return this->extra_fn_name;
}

std::string
DefInfo::get_full_name(void) const
{
    std::string full_name = this->llvm_fn_name;
    if (!this->extra_fn_name.empty())
    {
        full_name += this->extra_delim + this->extra_fn_name;
    }
    return full_name;
}

uint8_t
DefInfo::get_param_count(void) const
{
    return this->params_ty.size();
}

std::string
DefInfo::to_str(void) const
{
    auto oss = std::ostringstream { };
    oss << "DefInfo == ";
    oss << "LLVM OPC " << this->llvm_opcode << " - ";
    oss << "CMP OPC "
        << (this->cmp_opcode == static_cast<unsigned int>(-1)
                   ? "n/a"
                   : std::to_string(this->cmp_opcode))
        << " - ";
    oss << "EXTRA "
        << (this->extra_fn_name.empty() ? "n/a" : this->extra_fn_name) << " - ";
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
    fn_ptr(_fn_ptr)
{
    if (di->di_flags->is_fop)
    {
        this->bit_sz_min
            = def_ty_enum::def_ty_fl_bitsizes.at(di->params_ty.front());
        this->bit_sz_max = this->bit_sz_min;
    }
    else
    {
        this->bit_sz_min = Config::int_min_bit_sz;
        this->bit_sz_max = Config::int_max_bit_sz;
    }
};

/*******************************************************************************
 * EvalRunInfo
 ******************************************************************************/

EvalRunInfo::EvalRunInfo(const RunInfo& ri) :
    bit_sz_in_min(ri.bit_sz_min),
    bit_sz_in_max(ri.bit_sz_max),
    bit_sz_out(EvalRunInfo::get_out_bit_sz(ri)),
    di_flags(ri.di->di_flags) { };

EvalData::bit_sz_t
EvalRunInfo::get_out_bit_sz(const RunInfo& ri)
{
    if (def_ty_enum::is_def_ty_int(ri.di->ret_ty))
    {
        return ri.bit_sz_max;
    }
    else if (def_ty_enum::is_def_ty_float(ri.di->ret_ty))
    {
        return def_ty_enum::def_ty_fl_bitsizes.at(ri.di->ret_ty);
    }
    throw std::runtime_error(
        fmt::format("Unhandled DefInfo ret_ty `{}` for bit size conversion!",
            ri.di->ret_str));
}

/*******************************************************************************
 * InputData
 ******************************************************************************/

InputData::InputData(
    uint8_t _p_count, uint8_t _bit_sz, const DefInfoFlags* _di_flags) :
    parameter_count(_p_count)
{
    this->parameter_ranges.reserve(this->parameter_count);
    for (size_t i = 0; i < this->parameter_count; ++i)
    {
        parameter_ranges.emplace_back(InputDataRange {
            0, static_cast<uint64_t>(std::pow(2, _bit_sz)) - 1 });
    }

    if (_di_flags->is_div)
    {
        this->parameter_ranges.at(Config::divisor_idx).set_start(1);
    }
    else if (_di_flags->is_shift)
    {
        this->parameter_ranges.at(Config::divisor_idx).set_end(_bit_sz - 1);
    }
}

uint64_t
InputData::get_input_count(void)
{
    uint64_t acc = 1;
    for (const auto& idr : this->parameter_ranges)
    {
        acc *= idr.get_count();
    }
    return acc;
}

auto
InputData::get_input_range(uint8_t param) const
    -> decltype(this->parameter_ranges)::const_reference
{
    return this->parameter_ranges.at(param);
}

/*******************************************************************************
 * ThreadRunInfo
 ******************************************************************************/

ThreadRunInfo::ThreadRunInfo(const EvalRunInfo& _eri) :
    eri(&_eri),
    local_results(_eri.bit_sz_in_min, _eri.bit_sz_in_max, _eri.bit_sz_out) { };

/*******************************************************************************
 * Runner
 ******************************************************************************/

void
Runner::logs_os_init(void)
{
    this->log_fs_path = fmt::format(Config::out_log_path,
        std::chrono::round<std::chrono::seconds>(
            std::chrono::system_clock::now()));
    this->csv_fs_path = fmt::format(Config::out_csv_path,
        std::chrono::round<std::chrono::seconds>(
            std::chrono::system_clock::now()));

    this->log_fs = std::ofstream { this->log_fs_path };
    this->csv_fs = std::ofstream { this->csv_fs_path };
    this->csv_fs
        << "llvm_opcode,param_count,cmp_opcode,bit_size,uncertainty_coef\n";
}

void
Runner::logs_os_close(void)
{
    this->log_fs.close();
    this->csv_fs.close();
    DEBUG_PRINT(
        "== Log - `{}` == csv - `{}`\n", this->log_fs_path, this->csv_fs_path);
}

void
Runner::log_one_run(const EntropyResult& res, const DefInfo& def)
{
    this->log_fs << def.to_str() << '\n';
    this->log_fs << res.to_str();
    this->log_fs.flush();

    this->csv_fs << res.to_str_csv(def.llvm_fn_name, def.get_param_count());
    this->csv_fs.flush();
}

void
Runner::eval_threads_join(
    EvalData::Results& res, const std::span<ThreadRunInfo>& tris) const
{
    for (auto& tri : tris)
    {
        tri.thr.join();
        res.combine_results(tri.local_results);
        tri.~ThreadRunInfo();
    }
}

Runner::Runner(void)
{
    this->dl_hdl = dlopen((Config::lib_path).data(), RTLD_NOW);
    if (!this->dl_hdl)
    {
        std::cout << "Error opening library :: " << dlerror() << '\n';
        std::exit(1);
    }
}

Runner::~Runner(void) { dlclose(this->dl_hdl); }

void
Runner::init_all(void)
{
    auto buf = std::string { };
    auto def_ifs = std::ifstream { (Config::def_path).data() };
    this->logs_os_init();

    while (std::getline(def_ifs, buf))
    {
        if (buf.starts_with(Config::def_header_start))
        {
            continue;
        }

        this->defs.emplace_back(buf);
    }
}

const DefInfo&
Runner::get_def_info(std::string_view fn_name) const
{
    for (const auto& di : this->defs)
    {
        if (di.get_fn_name() == fn_name)
        {
            return di;
        }
    }

    throw std::runtime_error(
        fmt::format("Couldn't find DefInfo for name `{}`!", fn_name));
}

const EntropyResult
Runner::run_one(std::string_view fn_name) const
{
    return this->run_one(this->get_def_info(fn_name));
}

const EntropyResult
Runner::run_one(const DefInfo& di) const
{
    void* fn = dlsym(this->dl_hdl, di.get_fn_name().c_str());
    Utils::do_check(fn == nullptr,
        fmt::format("Couldn't find function `{}` in compiled library at `{}`!",
            di.get_fn_name(), Config::lib_path));

    auto ri = RunInfo { di, fn };
    return this->eval_ret(ri);
}

void
Runner::eval_all(void)
{
    for (const auto& di : this->defs)
    {
        DEBUG_PRINT(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[{}] ",
            std::chrono::round<std::chrono::seconds>(
                std::chrono::system_clock::now()));
        DEBUG_PRINT("{}\n", di.to_str());

        // TODO put this behind a flag
        if (di.get_extra() == "f32")
        {
            fmt::println(this->log_fs, "!! Skipping `float` function {}!",
                di.get_fn_name());
            continue;
        }

        const auto er = EntropyResult { this->run_one(di) };
        log_one_run(er, di);
    }

    this->logs_os_close();
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
        case def_ty_enum::I1:
            return this->eval_args<bool>(ri);
        case def_ty_enum::F32:
            return this->eval_args<float>(ri);
        case def_ty_enum::F16:
            return this->eval_args<_Float16>(ri);
        default:
            throw std::runtime_error(fmt::format("Unhandled return type {}!",
                def_ty_enum::to_str(ri.di->ret_ty)));
    }
}
