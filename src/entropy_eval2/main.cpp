#include "main.hpp"

int
main(void)
{
    auto eval_runner = Runner();
    eval_runner.eval_all();

    //eval_coefs_for_def_fns(def_path, dl_hdl, out_fd);

    //const char* time_buf = get_curr_time_str();
    //printf("[%s] END\n", time_buf);
    //free((char*) time_buf);

    return 0;
}
