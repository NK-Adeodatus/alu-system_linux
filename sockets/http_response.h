#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

void send_ok_response(int client_fd);
void send_status_response(int client_fd, int status_code,
			   const char *status_text);
void send_json_response(int client_fd, int status_code,
			 const char *status_text, const char *json);

#endif /* HTTP_RESPONSE_H */
