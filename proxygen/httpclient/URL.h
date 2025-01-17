// Copyright 2004-present Facebook.  All rights reserved.
#pragma once

#include <proxygen/lib/utils/ParseURL.h>

namespace proxygen { namespace httpclient {

/**
 * Struct representing a URL.
 */

class URL {
 public:
  explicit URL(const std::string& url = "") noexcept {
    valid_ = false;

    ParseURL parseUrl(url);

    scheme_ = parseUrl.scheme().str();
    host_ = parseUrl.hostNoBrackets().str();
    path_ = parseUrl.path().str();
    query_ = parseUrl.query().str();
    fragment_ = parseUrl.fragment().str();
    url_ = parseUrl.url().str();

    scheme_ = parseUrl.scheme().str();

    valid_ = (scheme_ == "http" || scheme_ == "https");

    if (parseUrl.port()) {
      port_ = parseUrl.port();
    } else {
      port_ = isSecure() ? 443 : 80;
    }

  }

  static std::string createUrl(
      const std::string& scheme,
      const std::string& hostAndPort,
      const std::string& path,
      const std::string& query,
      const std::string& fragment) noexcept {
    std::ostringstream out;
    out << scheme << "://" << hostAndPort << '/' << path;
    if (!query.empty()) {
      out << '?' << query;
    }
    if (!fragment.empty()) {
      out << '#' << fragment;
    }
    return out.str();
  }

  URL(const std::string scheme,
      const std::string host,
      uint16_t port,
      const std::string path = "",
      const std::string query = "",
      const std::string fragment = "") noexcept :
    scheme_(scheme),
    host_(host),
    port_(port),
    path_(path),
    query_(query),
    fragment_(fragment),
    url_(createUrl(scheme_, getHostAndPort(), path_, query_, fragment_)) {

    std::transform(scheme_.begin(),
                   scheme_.end(),
                   scheme_.begin(),
                   ::tolower);
    valid_ = (scheme == "http" || scheme == "https");
  }

  bool isValid() const noexcept {
    return valid_;
  }

  const std::string& getUrl() const noexcept {
    return url_;
  }

  uint16_t getPort() const noexcept {
    return port_;
  }

  const std::string& getScheme() const noexcept {
    return scheme_;
  }

  bool isSecure() const noexcept {
    return scheme_ == "https";
  }

  bool hasHost() const noexcept {
    return valid_ && !host_.empty();
  }

  const std::string& getHost() const noexcept {
    return host_;
  }

  std::string getHostAndPort() const noexcept {
    return port_ ? folly::to<std::string>(host_, ":", port_) : host_;
  }

  const std::string& getPath() const noexcept {
    return path_;
  }

  const std::string& getQuery() const noexcept {
    return query_;
  }

  const std::string& getFragment() const noexcept {
    return fragment_;
  }

 private:
  std::string scheme_;
  std::string host_;
  uint16_t port_;
  std::string path_;
  std::string query_;
  std::string fragment_;

  std::string url_;

  /* Does this represent a valid URL */
  bool valid_{false};
};

}}  // namespace proxygen::httpclient
