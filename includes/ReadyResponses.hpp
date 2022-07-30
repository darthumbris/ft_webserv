#ifndef READY_RESPONSES
# define READY_RESPONSES
# include <string>

std::string ngx_http_error_301_page =
"<html>" 
"<head><title>301 Moved Permanently</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>301 Moved Permanently</h1></center>" 
;


std::string ngx_http_error_302_page =
"<html>" 
"<head><title>302 Found</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>302 Found</h1></center>" 
;


std::string ngx_http_error_303_page =
"<html>" 
"<head><title>303 See Other</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>303 See Other</h1></center>" 
;


std::string ngx_http_error_307_page =
"<html>" 
"<head><title>307 Temporary Redirect</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>307 Temporary Redirect</h1></center>" 
;


std::string ngx_http_error_400_page =
"<html>" 
"<head><title>400 Bad Request</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>400 Bad Request</h1></center>" 
;


std::string ngx_http_error_401_page =
"<html>" 
"<head><title>401 Authorization Required</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>401 Authorization Required</h1></center>" 
;


std::string ngx_http_error_402_page =
"<html>" 
"<head><title>402 Payment Required</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>402 Payment Required</h1></center>" 
;


std::string ngx_http_error_403_page =
"<html>" 
"<head><title>403 Forbidden</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>403 Forbidden</h1></center>" 
;


std::string ngx_http_error_404_page =
"<html>" 
"<head><title>404 Not Found</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>404 Not Found</h1></center>" 
;


std::string ngx_http_error_405_page =
"<html>" 
"<head><title>405 Not Allowed</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>405 Not Allowed</h1></center>" 
;


std::string ngx_http_error_406_page =
"<html>" 
"<head><title>406 Not Acceptable</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>406 Not Acceptable</h1></center>" 
;


std::string ngx_http_error_408_page =
"<html>" 
"<head><title>408 Request Time-out</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>408 Request Time-out</h1></center>" 
;


std::string ngx_http_error_409_page =
"<html>" 
"<head><title>409 Conflict</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>409 Conflict</h1></center>" 
;


std::string ngx_http_error_410_page =
"<html>" 
"<head><title>410 Gone</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>410 Gone</h1></center>" 
;


std::string ngx_http_error_411_page =
"<html>" 
"<head><title>411 Length Required</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>411 Length Required</h1></center>" 
;


std::string ngx_http_error_412_page =
"<html>" 
"<head><title>412 Precondition Failed</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>412 Precondition Failed</h1></center>" 
;


std::string ngx_http_error_413_page =
"<html>" 
"<head><title>413 Request Entity Too Large</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>413 Request Entity Too Large</h1></center>" 
;


std::string ngx_http_error_414_page =
"<html>" 
"<head><title>414 Request-URI Too Large</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>414 Request-URI Too Large</h1></center>" 
;


std::string ngx_http_error_415_page =
"<html>" 
"<head><title>415 Unsupported Media Type</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>415 Unsupported Media Type</h1></center>" 
;


std::string ngx_http_error_416_page =
"<html>" 
"<head><title>416 Requested Range Not Satisfiable</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>416 Requested Range Not Satisfiable</h1></center>" 
;


std::string ngx_http_error_494_page =
"<html>" 
"<head><title>400 Request Header Or Cookie Too Large</title></head>"

"<body bgcolor=\"white\">" 
"<center><h1>400 Bad Request</h1></center>" 
"<center>Request Header Or Cookie Too Large</center>" 
;


std::string ngx_http_error_495_page =
"<html>" 
"<head><title>400 The SSL certificate error</title></head>"

"<body bgcolor=\"white\">" 
"<center><h1>400 Bad Request</h1></center>" 
"<center>The SSL certificate error</center>" 
;


std::string ngx_http_error_496_page =
"<html>" 
"<head><title>400 No required SSL certificate was sent</title></head>"

"<body bgcolor=\"white\">" 
"<center><h1>400 Bad Request</h1></center>" 
"<center>No required SSL certificate was sent</center>" 
;


std::string ngx_http_error_497_page =
"<html>" 
"<head><title>400 The plain HTTP request was sent to HTTPS port</title></head>"

"<body bgcolor=\"white\">" 
"<center><h1>400 Bad Request</h1></center>" 
"<center>The plain HTTP request was sent to HTTPS port</center>" 
;


std::string ngx_http_error_500_page =
"<html>" 
"<head><title>500 Internal Server Error</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>500 Internal Server Error</h1></center>" 
;


std::string ngx_http_error_501_page =
"<html>" 
"<head><title>501 Not Implemented</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>501 Not Implemented</h1></center>" 
;


std::string ngx_http_error_502_page =
"<html>" 
"<head><title>502 Bad Gateway</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>502 Bad Gateway</h1></center>" 
;


std::string ngx_http_error_503_page =
"<html>" 
"<head><title>503 Service Temporarily Unavailable</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>503 Service Temporarily Unavailable</h1></center>" 
;


std::string ngx_http_error_504_page =
"<html>" 
"<head><title>504 Gateway Time-out</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>504 Gateway Time-out</h1></center>" 
;


std::string ngx_http_error_507_page =
"<html>" 
"<head><title>507 Insufficient Storage</title></head>" 
"<body bgcolor=\"white\">" 
"<center><h1>507 Insufficient Storage</h1></center>" 
;

#endif
