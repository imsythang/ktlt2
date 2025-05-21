/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any
// other libraries.

#include "hcmcampaign.h"

using namespace std;

void g_satc_01() {
  cout << "----- Sample Testcase 01 -----" << endl;
  Configuration *config = new Configuration("config.txt");
  cout << config->str() << endl;
  delete config;
}
int main(int argc, const char *argv[]) {
  g_satc_01();
  return 0;
}
