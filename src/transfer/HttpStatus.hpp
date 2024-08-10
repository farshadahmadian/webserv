/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnavidd <nnavidd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:27:50 by nnavidd           #+#    #+#             */
/*   Updated: 2024/07/16 20:04:39 by nnavidd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  The #pragma once directive has a very simple concept.
    The header file containing this directive is included only 
    once even if the programmer includes it multiple times during a compilation.
    This directive works similar to the #include guard idiom.
    Use of #pragma once saves the program from multiple inclusion optimisation.*/
#pragma once

/* ************************************************************************** */
/*                                    INFO                                    */
/* ************************************************************************** */
#define STATUS_100 "HTTP/1.1 100 Continue\r\n"
#define STATUS_101 "HTTP/1.1 101 Switching Protocols\r\n"
#define STATUS_102 "HTTP/1.1 102 Processing\r\n"
#define STATUS_103 "HTTP/1.1 103 Early Hints\r\n"
/* ************************************************************************** */
/*                                  SUCCESS                                   */
/* ************************************************************************** */
#define STATUS_200 "HTTP/1.1 200 OK\r\n"
#define STATUS_201 "HTTP/1.1 201 Created\r\n"
#define STATUS_202 "HTTP/1.1 202 Accepted\r\n"
#define STATUS_203 "HTTP/1.1 203 Non-Authoritative Information\r\n"
#define STATUS_204 "HTTP/1.1 204 No Content\r\n"
#define STATUS_205 "HTTP/1.1 205 Reset Content\r\n"
#define STATUS_206 "HTTP/1.1 206 Partial Content\r\n"
#define STATUS_207 "HTTP/1.1 207 Multi-Status\r\n"
#define STATUS_208 "HTTP/1.1 208 Already Reported\r\n"
#define STATUS_226 "HTTP/1.1 226 IM Used\r\n"
/* ************************************************************************** */
/*                                REDIRECTION                                 */
/* ************************************************************************** */
#define STATUS_300 "HTTP/1.1 300 Multiple Choices\r\n"
#define STATUS_301 "HTTP/1.1 301 Moved Permanently\r\n"
#define STATUS_302 "HTTP/1.1 302 Found\r\n"
#define STATUS_303 "HTTP/1.1 303 See Other\r\n"
#define STATUS_304 "HTTP/1.1 304 Not Modified\r\n"
#define STATUS_307 "HTTP/1.1 307 Temporary Redirect\r\n"
#define STATUS_308 "HTTP/1.1 308 Permanent Redirect\r\n"
/* ************************************************************************** */
/*                                CLIENT ERROR                                */
/* ************************************************************************** */
#define STATUS_400 "HTTP/1.1 400 Bad Request\r\n"
#define STATUS_401 "HTTP/1.1 401 Unauthorized\r\n"
#define STATUS_402 "HTTP/1.1 402 Payment Required\r\n"
#define STATUS_403 "HTTP/1.1 403 Forbidden\r\n"
#define STATUS_404 "HTTP/1.1 404 Not Found\r\n"
#define STATUS_405 "HTTP/1.1 405 Method Not Allowed\r\n"
#define STATUS_406 "HTTP/1.1 406 Not Acceptable\r\n"
#define STATUS_407 "HTTP/1.1 407 Proxy Authentication Required\r\n"
#define STATUS_408 "HTTP/1.1 408 Request Timeout\r\n"
#define STATUS_409 "HTTP/1.1 409 Conflict\r\n"
#define STATUS_410 "HTTP/1.1 410 Gone\r\n"
#define STATUS_411 "HTTP/1.1 411 Length Required\r\n"
#define STATUS_412 "HTTP/1.1 412 Precondition Failed\r\n"
#define STATUS_413 "HTTP/1.1 413 Content Too Large\r\n"
#define STATUS_414 "HTTP/1.1 414 URI Too Long\r\n"
#define STATUS_415 "HTTP/1.1 415 Unsupported Media Type\r\n"
#define STATUS_416 "HTTP/1.1 416 Range Not Satisfiable\r\n"
#define STATUS_417 "HTTP/1.1 417 Expectation Failed\r\n"
#define STATUS_418 "HTTP/1.1 418 I'm a teapot\r\n"
#define STATUS_421 "HTTP/1.1 421 Misdirected Request\r\n"
#define STATUS_422 "HTTP/1.1 422 Unprocessable Content\r\n"
#define STATUS_423 "HTTP/1.1 423 Locked\r\n"
#define STATUS_424 "HTTP/1.1 424 Failed Dependency\r\n"
#define STATUS_425 "HTTP/1.1 425 Too Early\r\n"
#define STATUS_426 "HTTP/1.1 426 Upgrade Required\r\n"
#define STATUS_428 "HTTP/1.1 428 Precondition Required\r\n"
#define STATUS_429 "HTTP/1.1 429 Too Many Requests\r\n"
#define STATUS_431 "HTTP/1.1 431 Request Header Fields Too Large\r\n"
#define STATUS_451 "HTTP/1.1 451 Unavailable For Legal Reasons\r\n"
/* ************************************************************************** */
/*                                SERVER ERROR                                */
/* ************************************************************************** */
#define STATUS_500 "HTTP/1.1 500 Internal Server Error\r\n"
#define STATUS_501 "HTTP/1.1 501 Not Implemented\r\n"
#define STATUS_502 "HTTP/1.1 502 Bad Gateway\r\n"
#define STATUS_503 "HTTP/1.1 503 Service Unavailable\r\n"
#define STATUS_504 "HTTP/1.1 504 Gateway Timeout\r\n"
#define STATUS_505 "HTTP/1.1 505 HTTP Version Not Supported\r\n"
#define STATUS_506 "HTTP/1.1 506 Variant Also Negotiates\r\n"
#define STATUS_507 "HTTP/1.1 507 Insufficient Storage\r\n"
#define STATUS_508 "HTTP/1.1 508 Loop Detected\r\n"
#define STATUS_510 "HTTP/1.1 510 Not Extended\r\n"
#define STATUS_511 "HTTP/1.1 511 Network Authentication Required\r\n"