#ifndef LIB_WINDOW_H_
#define LIB_WINDOW_H_

#include "lib/deduce.h"

#include <mutex>
#include <vector>

extern std::mutex redraw_mutex;
extern bool should_redraw;

void notify_exit();
void run_window(std::vector<Node_Ptr> const& roots);

#endif
