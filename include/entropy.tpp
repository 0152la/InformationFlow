/*******************************************************************************
 * IF_Histogram_Entry
 ******************************************************************************/

template<typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert(O _output)
{
    this->insert_many(_output, 1);
}

template<typename I, typename O>
void
IF_Histogram_Entry<I, O>::insert_many(O _output, uint64_t _out_count)
{
    if (auto entry = this->outputs.find(_output); entry != this->outputs.end())
    {
        entry->second += _out_count;
    }
    else
    {
        this->outputs.insert({ _output, _out_count });
    }
    this->out_count += _out_count;
}

template<typename I, typename O>
void
IF_Histogram_Entry<I, O>::print(void)
{
    std::cout << "== INPUT " << this->input << " -- OUTPUTS [";
    std::ostringstream vals(
        this->outputs.empty() ? "" : std::to_string(this->outputs.at(0)),
        std::ios_base::ate);

    auto outs = this->outputs.begin();
    std::advance(outs, 1);
    for (; outs != this->outputs.end(); ++outs)
    {
        vals << ", <" << outs->first << ", " << outs->second << ">";
    }
    std::cout << vals.str() << "]" << std::endl;
}

