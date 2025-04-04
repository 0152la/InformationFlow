double
IF_FuzzEngine::get_fuzzed_entropy(llvm::Instruction instr)
{
    IF_Histogram fuzz_hist;

    return fuzz_hist.calculate_uncertainty_coefficient_out_given_in();
}
