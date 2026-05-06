using Plots
using Polynomials
using Interpolations
using CSV

const expected_args = 1

const out_dir = "graphs"
const out_graph_prefix = "plot-"
const log_name = "jl_llvm.log"

const bit_sz_ceil = 64
const bit_sz_floor = 2
bit_sz_max = bit_sz_floor
bit_sz_min = bit_sz_ceil

function parse_csv_data(data)
    uc_vals = Dict{Int64, Float64}()
    fn_uc_vals = Dict{String, typeof(uc_vals)}()
    for row in data
        if !(row.fn_name in keys(fn_uc_vals))
            fn_uc_vals[row.fn_name] = typeof(uc_vals)()
        end
        fn_uc_vals[row.fn_name][row.bit_sz] = row.uc_val
        global bit_sz_min = row.bit_sz < bit_sz_min ? row.bit_sz : bit_sz_min
        global bit_sz_max = row.bit_sz > bit_sz_max ? row.bit_sz : bit_sz_max
    end
    @assert bit_sz_max <= bit_sz_ceil
    return fn_uc_vals
end

function log_fn_data(fn_name, ucs, fn_fit)
    println("FN $(fn_name) == Polynomial -- $(fn_fit)")
    # println("\t$(keys(ucs)) -- $(collect(keys(ucs)))")
    # println("\t$(values(ucs)) -- $(collect(values(ucs)))")
    # for bit_sz in sort(collect(keys(ucs)))
        # println("\t * Bit Size $(bit_sz) -- $(ucs[bit_sz])")
    # end

    curr_bit_sz = bit_sz_floor
    while curr_bit_sz <= bit_sz_ceil
        if curr_bit_sz in keys(ucs)
            val = ucs[curr_bit_sz]
        else
            val = fn_fit(curr_bit_sz)
        end
        println("\t * Bit Size $(curr_bit_sz) -- $(val)")
        curr_bit_sz += 1
    end
end

function emit_plots(data)
    mkpath(out_dir)
    for (fn_name, ucs) in data
        if ucs[bit_sz_min] < ucs[bit_sz_max]
            ucs[bit_sz_ceil] = 0.5
        elseif ucs[bit_sz_min] == ucs[bit_sz_max]
            ucs[bit_sz_ceil] = ucs[bit_sz_min]
        else
            ucs[bit_sz_ceil] = 0.0
        end

        vs = [(k, ucs[k]) for k in sort(collect(keys(ucs)))]
        xs = collect(getindex.(vs, 1))
        ys = collect(getindex.(vs, 2))
        # ys = [ucs[k] for k in sort(collect(keys(ucs)))]
        println("VS == $(vs)")
        println("XS == $(xs)")
        println("YS == $(ys)")
        fn_plot = scatter(xs, ys, markerstrokewidth=0, label="Data", title="Function `$(fn_name)`", ylims = (0, 1.1), xlims = (bit_sz_floor - 1, bit_sz_ceil + 1))

        if (length(ucs)) != 1
            # fn_fit = linear_interpolation(xs, ys, extrapolation_bc=Line());
            fn_fit = cubic_spline_interpolation(xs, ys);
            log_fn_data(fn_name, ucs, fn_fit)
            # plot!(fn_plot, fn_fit, extrema(xs)..., label="Interpolation")
            plot!(fn_plot, fn_fit, (bit_sz_floor-0.5):0.1:(bit_sz_ceil+0.5), label = "Interpolation")
        end
        Plots.png(fn_plot, joinpath(out_dir, "$(out_graph_prefix * fn_name)"))
    end
end

if length(ARGS) != expected_args
    println("Expected $(expected_args) arguments; $(length(ARGS)) given!")
    exit(1)
end

csv_file_path = ARGS[1]
csv_col_types = [String, UInt16, Float64]
csv_col_names = ["fn_name", "bit_sz", "uc_val"]
csv_data_raw = CSV.Rows(csv_file_path; header = csv_col_names, types = csv_col_types, skipto = 2, strict = true, reusebuffer = true)
csv_data = parse_csv_data(csv_data_raw)
emit_plots(csv_data)
