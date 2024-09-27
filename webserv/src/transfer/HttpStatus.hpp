/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnabaeei <nnabaeei@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:27:50 by nnavidd           #+#    #+#             */
/*   Updated: 2024/09/06 19:47:25 by nnabaeei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                    INFO                                    */
/* ************************************************************************** */
#define STATUS_100 "HTTP/1.1 100 Continue"
#define STATUS_101 "HTTP/1.1 101 Switching Protocols"
#define STATUS_102 "HTTP/1.1 102 Processing"
#define STATUS_103 "HTTP/1.1 103 Early Hints"
/* ************************************************************************** */
/*                                  SUCCESS                                   */
/* ************************************************************************** */
#define STATUS_200 "HTTP/1.1 200 OK"
#define STATUS_201 "HTTP/1.1 201 Created"
#define STATUS_202 "HTTP/1.1 202 Accepted"
#define STATUS_203 "HTTP/1.1 203 Non-Authoritative Information"
#define STATUS_204 "HTTP/1.1 204 No Content"
#define STATUS_205 "HTTP/1.1 205 Reset Content"
#define STATUS_206 "HTTP/1.1 206 Partial Content"
#define STATUS_207 "HTTP/1.1 207 Multi-Status"
#define STATUS_208 "HTTP/1.1 208 Already Reported"
#define STATUS_226 "HTTP/1.1 226 IM Used"
/* ************************************************************************** */
/*                                REDIRECTION                                 */
/* ************************************************************************** */
#define STATUS_300 "HTTP/1.1 300 Multiple Choices"
#define STATUS_301 "HTTP/1.1 301 Moved Permanently"
#define STATUS_302 "HTTP/1.1 302 Found"
#define STATUS_303 "HTTP/1.1 303 See Other"
#define STATUS_304 "HTTP/1.1 304 Not Modified"
#define STATUS_307 "HTTP/1.1 307 Temporary Redirect"
#define STATUS_308 "HTTP/1.1 308 Permanent Redirect"
/* ************************************************************************** */
/*                                CLIENT ERROR                                */
/* ************************************************************************** */
#define STATUS_400 "HTTP/1.1 400 Bad Request"
#define STATUS_401 "HTTP/1.1 401 Unauthorized"
#define STATUS_402 "HTTP/1.1 402 Payment Required"
#define STATUS_403 "HTTP/1.1 403 Forbidden"
#define STATUS_404 "HTTP/1.1 404 Not Found"
#define STATUS_405 "HTTP/1.1 405 Method Not Allowed"
#define STATUS_406 "HTTP/1.1 406 Not Acceptable"
#define STATUS_407 "HTTP/1.1 407 Proxy Authentication Required"
#define STATUS_408 "HTTP/1.1 408 Request Timeout"
#define STATUS_409 "HTTP/1.1 409 Conflict"
#define STATUS_410 "HTTP/1.1 410 Gone"
#define STATUS_411 "HTTP/1.1 411 Length Required"
#define STATUS_412 "HTTP/1.1 412 Precondition Failed"
#define STATUS_413 "HTTP/1.1 413 Content Too Large"
#define STATUS_414 "HTTP/1.1 414 URI Too Long"
#define STATUS_415 "HTTP/1.1 415 Unsupported Media Type"
#define STATUS_416 "HTTP/1.1 416 Range Not Satisfiable"
#define STATUS_417 "HTTP/1.1 417 Expectation Failed"
#define STATUS_418 "HTTP/1.1 418 I'm a teapot"
#define STATUS_421 "HTTP/1.1 421 Misdirected Request"
#define STATUS_422 "HTTP/1.1 422 Unprocessable Content"
#define STATUS_423 "HTTP/1.1 423 Locked"
#define STATUS_424 "HTTP/1.1 424 Failed Dependency"
#define STATUS_425 "HTTP/1.1 425 Too Early"
#define STATUS_426 "HTTP/1.1 426 Upgrade Required"
#define STATUS_428 "HTTP/1.1 428 Precondition Required"
#define STATUS_429 "HTTP/1.1 429 Too Many Requests"
#define STATUS_431 "HTTP/1.1 431 Request Header Fields Too Large"
#define STATUS_451 "HTTP/1.1 451 Unavailable For Legal Reasons"
/* ************************************************************************** */
/*                                SERVER ERROR                                */
/* ************************************************************************** */
#define STATUS_500 "HTTP/1.1 500 Internal Server Error"
#define STATUS_501 "HTTP/1.1 501 Not Implemented"
#define STATUS_502 "HTTP/1.1 502 Bad Gateway"
#define STATUS_503 "HTTP/1.1 503 Service Unavailable"
#define STATUS_504 "HTTP/1.1 504 Gateway Timeout"
#define STATUS_505 "HTTP/1.1 505 HTTP Version Not Supported"
#define STATUS_506 "HTTP/1.1 506 Variant Also Negotiates"
#define STATUS_507 "HTTP/1.1 507 Insufficient Storage"
#define STATUS_508 "HTTP/1.1 508 Loop Detected"
#define STATUS_510 "HTTP/1.1 510 Not Extended"
#define STATUS_511 "HTTP/1.1 511 Network Authentication Required"