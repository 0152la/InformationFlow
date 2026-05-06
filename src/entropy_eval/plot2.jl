import CSV
import Plots
import Interpolations
import LsqFit


csv_path_default = "./entropy_out-20260409-142539.csv"
out_dir_default = "./graphs"

max_bit_sz = 64

################################################################################
# Function Uncertainty Coefficient Data struct
################################################################################

mutable struct FuncUCData
    name::String
    data::Dict{UInt16, Float64} # {bit_sz, uc}
    in_params::UInt16
    interp::Interpolations.AbstractInterpolation
    all_same::Bool

    FuncUCData(_name, _data::Dict{UInt16, Float64}, _params) = new(_name, _data, _params)
end

function init_fndata_interp(d::FuncUCData)
    d.all_same = all_vals_same(d)
    if !d.all_same
        ex_data = sort(merge(d.data, Dict([(max_bit_sz, approx_max_val(d))])), by=x->x[1])
        d.interp = Interpolations.linear_interpolation(collect(keys(ex_data)), collect(values(ex_data)); extrapolation_bc = Interpolations.Line())
    end
end

function get_bit_sz_limits(d::FuncUCData)
    return collect(extrema(keys(d.data)))
end

function approx_max_val(d::Dict{UInt16, Float64}, p::UInt16)
    last_val = maximum(d)
    last_diff = last_val[2] - d[last_val[1] - 1]
    approx_lim = last_val[2] + (max_bit_sz - last_val[1]) * last_diff

    println("LAST_VAL $last_val[2] -- DIFF $last_diff -- APPROX LIM $approx_lim -- CLAMPED $(clamp(approx_lim, 0.0, 1.0 / p))")
    return clamp(approx_lim, 0.0, 1.0 / p)
end

function approx_max_val(d::FuncUCData)
    return approx_max_val(d.data, d.in_params)
end

function all_vals_same(d::FuncUCData)
    eq_val = minimum(d.data)[2]
    for v in values(d.data)
        if v != eq_val
            return false
        end
    end
    return true
end

function linear_extrapolate(d::FuncUCData, bit_sz::UInt16)
    if d.all_same
        return values(d.data)[0]
        return collect(values(d.data))[0]
    end

    if bit_sz in keys(d.data)
        return d.data[bit_sz]
    end

    if bit_sz < minimum(d.data)[1] || bit_sz > max_bit_sz
        throw(BoundsError("given bit_sz `$bit_sz` outside expected bounds"))
    end

    return d.interp(bit_sz)

    # last_val = maximum(d.data)
    # return (approx_max_val(d) - last_val[2]) / (max_bit_sz - last_val[1]) * bit_sz
end

################################################################################
# Work functions
################################################################################

function parse_csv_data(path)
    parsed = Dict{String, FuncUCData}()

    csv_data_cols = ["fn_name", "param_count", "bit_sz", "u_coef"]
    csv_data_cols_ty = [String, UInt16, UInt16, Float64]
    csv_data_raw = CSV.Rows(path; header = csv_data_cols, types = csv_data_cols_ty, skipto = 2, reusebuffer = true, strict = true)

    for row in csv_data_raw
        if !(row.fn_name in keys(parsed))
            parsed[row.fn_name] = FuncUCData(row.fn_name, fieldtype(FuncUCData, :data)(), row.param_count)
        end
        parsed[row.fn_name].data[row.bit_sz] = row.u_coef
    end

    for fn_data in values(parsed)
        init_fndata_interp(fn_data)
    end

    return values(parsed)
end

function draw_data(data)
    if (Base.Filesystem.isdir(out_dir))
        Base.Filesystem.rm(out_dir; recursive = true)
    end
    Base.Filesystem.mkdir(out_dir)

    for one_fn in data
        if length(one_fn.data) < 2
            println("Skipping function `$(one_fn.name)` with insufficient samples.")
            continue
        end

        if all_vals_same(one_fn)
            println("Skipping function `$(one_fn.name)` with constant value.")
            continue
        end

        # Raw data, scattered
        xs = collect(keys(one_fn.data))
        ys = collect(values(one_fn.data))
        println("FN $(one_fn.name) == KEYS -- $(xs) -- EXTREMA $(extrema(xs)) == VALS -- $(ys)")
        Plots.plot(xs, ys; seriestype=:scatter)

        # Linear interpolated data
        bit_sz_lim = max_bit_sz
        xs_interp = (maximum(one_fn.data)[1] + 1):bit_sz_lim
        ys_interp = [linear_extrapolate(one_fn, UInt16(b)) for b in xs_interp]
        Plots.plot!(xs_interp, ys_interp; seriestype=:scatter, label="Linear interpolation")

        # Fitted data
        lim = approx_max_val(one_fn)

        # @. model(x, p) = lim + p[1] * exp(-p[2] * x)
        # a0 = Float64(minimum(one_fn.data)[1] - lim)
        # k0 = one_fn.data[minimum(one_fn.data)[1] + 1] - lim
        # k0 /= minimum(one_fn.data)[2] - lim
        # k0 = -log(k0)
        # p0 = [a0, k0]

        @. model(x, p) = p[1] * exp(-p[2] * x)
        a0, k0 = 0.5, 0.5
        println("Estimated fit params == a $a0 == k $k0")
        fn_fit = LsqFit.curve_fit(model, Float64.([xs; bit_sz_lim]), [ys; lim], [a0, k0])
        a_fit, k_fit = fn_fit.param
        println("Fit params: $a_fit -- $k_fit")
        xs_extrap = maximum(one_fn.data)[1]:bit_sz_lim
        ys_extrap = model(xs_extrap, fn_fit.param)
        Plots.plot!(xs_extrap, ys_extrap, label="Exponential decay")


        # General settings
        Plots.plot!(title = one_fn.name, xlabel = "Bit size", ylabel = "Uncertainty coefficient", size=(1500, 1000))
        Plots.plot!(xlims = ([minimum(keys(one_fn.data)), bit_sz_lim] + [-1, 1]), ylims = (-0.1, 1.1))

        # Output
        Plots.savefig("./graphs/$(one_fn.name)-plot.png")
        # Plots.savefig("./plot.png")
        # break

        # Emit csv with extrapolations
        open("./graphs/$(one_fn.name)-out.csv", "w") do out_fd
            curr_idx = minimum(one_fn.data)[1]
            while curr_idx <= bit_sz_lim
                if curr_idx in keys(one_fn.data)
                    val = one_fn.data[curr_idx]
                else
                    val = model(curr_idx, fn_fit.param)
                    val = "$val, $(linear_extrapolate(one_fn, UInt16(curr_idx)))"
                end
                println(out_fd, "$(curr_idx),$(val)")
                curr_idx += 1
            end
        end

    end
end

################################################################################
# Main
################################################################################

ARGC = length(ARGS)
csv_path = ARGC >= 1 ? ARGS[1] : csv_path_default
out_dir = ARGC >= 2 ? ARGS[2] : out_dir_default

println("== Using input `$csv_path` and output `$out_dir`...")

parsed_data = parse_csv_data(csv_path)
for fn_data in parsed_data
    println("Fn name $(fn_data.name) -- Lims $(get_bit_sz_limits(fn_data))")
end
draw_data(parsed_data)

println("== Done == input `$(csv_path)` == output `$(out_dir)`")
