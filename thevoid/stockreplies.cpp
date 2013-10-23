/*
 * 2013+ Copyright (c) Ruslan Nigatullin <euroelessar@yandex.ru>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "stockreplies_p.hpp"

namespace ioremap {
namespace thevoid {

namespace stock_replies {

template <size_t N>
static boost::asio::const_buffer to_buffer(const char (&str)[N])
{
	return boost::asio::buffer(str, N - 1);
}

namespace status_strings {

#define CASE_CODE(code, message) case code: return stock_replies::to_buffer("HTTP/1.1 " #code " " message "\r\n")

boost::asio::const_buffer to_buffer(int status)
{
	switch (status) {
		CASE_CODE(100, "Continue");
		CASE_CODE(101, "Switching Protocols");
		CASE_CODE(102, "Processing");

		CASE_CODE(200, "OK");
	        CASE_CODE(201, "Created");
		CASE_CODE(202, "Accepted");
		CASE_CODE(203, "Non-Authoritative Information");
		CASE_CODE(204, "No Content");
		CASE_CODE(205, "Reset Content");
		CASE_CODE(206, "Partial Content");
		CASE_CODE(207, "Multi-Status");
		CASE_CODE(208, "Already Reported");
		CASE_CODE(209, "IM Used");

	        CASE_CODE(300, "Multiple Choices");
	        CASE_CODE(301, "Moved Permanently");
	        CASE_CODE(302, "Moved Temporarily");
		CASE_CODE(304, "Not Modified");
		CASE_CODE(305, "Use Proxy");
		CASE_CODE(306, "Switch Proxy");
		CASE_CODE(307, "Temporary Redirect");
		CASE_CODE(308, "Permanent Redirect");

		CASE_CODE(400, "Bad Request");
		CASE_CODE(401, "Unauthorized");
		CASE_CODE(402, "Payment Required");
		CASE_CODE(403, "Forbidden");
		CASE_CODE(404, "Not Found");
		CASE_CODE(405, "Method Not Allowed");
		CASE_CODE(406, "Not Acceptable");
		CASE_CODE(407, "Proxy Authentication Required");
		CASE_CODE(408, "Request Timeout");
		CASE_CODE(409, "Conflict");
		CASE_CODE(410, "Gone");
		CASE_CODE(411, "Length Required");
		CASE_CODE(412, "Precondition Failed");
		CASE_CODE(413, "Request Entity Too Large");
		CASE_CODE(414, "Request-URI Too Long");
		CASE_CODE(415, "Unsupported Media Type");
		CASE_CODE(416, "Requested Range Not Satisfiable");
		CASE_CODE(417, "Expectation Failed");
		CASE_CODE(418, "I'm a teapot");
		CASE_CODE(419, "Authentication Timeout");
		CASE_CODE(422, "Unprocessable Entity");
		CASE_CODE(423, "Locked");
		CASE_CODE(424, "Failed Dependency");
//		CASE_CODE(424, "Method Failure"); -- WebDav
//		CASE_CODE(425, "Unordered Collection"); -- Internet draft
		CASE_CODE(426, "Upgrade Required");
		CASE_CODE(428, "Precondition Required");
		CASE_CODE(429, "Too Many Requests");
		CASE_CODE(431, "Request Header Fields Too Large");
		CASE_CODE(444, "No Response");
//		CASE_CODE(451, "Unavailable For Legal Reasons"); -- Internet draft

		default:
		CASE_CODE(500, "Internal Server Error");
		CASE_CODE(501, "Not Implemented");
		CASE_CODE(502, "Bad Gateway");
		CASE_CODE(503, "Service Unavailable");
		CASE_CODE(504, "Gateway Timeout");
		CASE_CODE(505, "HTTP Version Not Supported");
		CASE_CODE(506, "Variant Also Negotiates");
		CASE_CODE(507, "Insufficient Storage");
		CASE_CODE(508, "Loop Detected");
		CASE_CODE(510, "Not Extended");
		CASE_CODE(511, "Network Authentication Required");
		CASE_CODE(522, "Connection timed out");
	}
}

#undef CASE_CODE

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

boost::asio::const_buffer status_to_buffer(swarm::http_response::status_type status)
{
    return status_strings::to_buffer(status);
}

swarm::http_response stock_reply(swarm::http_response::status_type status)
{
	swarm::http_response reply;
	reply.set_code(status);
	reply.headers().set_content_length(0);
	return reply;
}

std::vector<boost::asio::const_buffer> to_buffers(const swarm::http_response &reply, const boost::asio::const_buffer &content)
{
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(status_strings::to_buffer(reply.code()));

	const auto &headers = reply.headers().all();
	for (std::size_t i = 0; i < headers.size(); ++i) {
		auto &header = headers[i];
		buffers.push_back(boost::asio::buffer(header.first));
		buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
		buffers.push_back(boost::asio::buffer(header.second));
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	}
	buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	buffers.push_back(content);
	return buffers;
}

} // namespace stock_replies

} } // namespace ioremap::thevoid
