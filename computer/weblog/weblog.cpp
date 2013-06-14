/*
 * This file is part of the Ckelinar Project.
 *
 * Copyright (C) 2013  UNIVERSIDAD DE CHILE.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include "mongoose.h"
using std::string;
using std::stringstream;
using std::vector;
using std::endl;

vector<string> log;

// This function will be called by mongoose on every new request.
static int begin_request_handler(struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);

  string uri(request_info->uri);
  string query;
  if (request_info->query_string)
    query = request_info->query_string;

  if (!query.empty()) {
    log.push_back(query);
  }

  stringstream output;

  output << "[ ";
  for(int i=0; i<log.size(); i++) {
    output << "\"" << log[i] << "\"";
    if (i!=log.size()-1)
      output << "," << endl;
  }
  output << "] ";

  // Send HTTP reply to the client
  mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %lu\r\n"        // Always set Content-Length
            "\r\n"
            "%s",
            output.str().length(), output.str().c_str());

  // Returning non-zero tells mongoose that our function has replied to
  // the client, and mongoose should not send client any more data.
  return 1;
}

int main(int argc, char **argv) {
  struct mg_context *ctx;
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {"listening_ports", "8080", NULL};

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  // Start the web server.
  ctx = mg_start(&callbacks, NULL, options);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  mg_stop(ctx);

  return 0;
}
