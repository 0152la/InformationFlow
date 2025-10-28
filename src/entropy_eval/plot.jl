using Plots
using Polynomials
using CSV

const expected_args = 1

const out_dir = "graphs"
const out_graph_prefix = "plot-"
const log_name = "jl_llvm.log"

function parse_csv_to_dict(data)
    uc_vals = Dict{Int64, Float64}()
    fn_uc_vals = Dict{String, typeof(uc_vals)}()
    for row in data
        if !(row.fn_name in keys(fn_uc_vals))
            fn_uc_vals[row.fn_name] = typeof(uc_vals)()
        end
        fn_uc_vals[row.fn_name][row.bit_count] = row.uc_val
    end
    return fn_uc_vals
end

function emit_plots(data)
    for (fn_name, ucs) in data
        println("FN $(fn_name) ==")
        println("\t$(keys(ucs)) -- $(collect(keys(ucs)))")
        println("\t$(values(ucs)) -- $(collect(values(ucs)))")

        xs = collect(keys(ucs))
        ys = collect(values(ucs))
        fn_fit = Polynomials.fit(xs, ys)
        fn_plot = scatter(xs, ys, markerstrokewidth=0, label="Data", title="Function `$(fn_name)`")
        plot!(fn_plot, fn_fit, extrema(xs)..., label="Interpolation")
        Plots.png(fn_plot, joinpath(out_dir, "$(out_graph_prefix * fn_name)"))
    end
end

if length(ARGS) != expected_args
    println("Expected $(expected_args) arguments; $(length(ARGS)) given!")
    exit(1)
end

csv_file_path = ARGS[1]
csv_data = CSV.Rows(csv_file_path; header = ["fn_name", "bit_count", "uc_val"], types = [String, Int64, Float64], strict = true, reusebuffer = true)

mkpath(out_dir)
csv_dict = parse_csv_to_dict(csv_data)

emit_plots(csv_dict)
