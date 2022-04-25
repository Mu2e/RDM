#include "RDM/inc/RdmPriority.hh"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>

using namespace mu2e;

//***********************************************************
RdmPriority::RdmPriority(RdmConfig &config) : _config(config) {
  StringVec its = splitString(config.priorities(), ',');
  StringVec jts;
  for (auto const &it : its) {
    jts = splitString(it, '=');
    // pritem pp {jts[0],std::stoul(jts[1])};
    _items.push_back(pritem{jts[0], std::stoul(jts[1])});
  }

  return;
}

//***********************************************************
RdmPriority::StringVec RdmPriority::splitString(const std::string &line,
                                                char del) const {
  using namespace boost;

  StringVec sv;

  escaped_list_separator<char> els('\\', del, '"');
  tokenizer<escaped_list_separator<char>> tok(line, els);
  for (tokenizer<escaped_list_separator<char>>::iterator beg = tok.begin();
       beg != tok.end(); ++beg) {
    std::string ss(*beg);
    boost::trim(ss);
    sv.emplace_back(ss);
  }

  return sv;
}

//***********************************************************

size_t RdmPriority::priority(std::string const &fn) const {
  for (auto const &pi : _items) {
    if (is_match(fn.c_str(), pi.test.c_str())) {
      return pi.priority;
      /*
      if(config().verbose()>8) {
        std::string message(80);
        message += "Priority ";
        message += std::to_string( pi.priority);
        message += " assgined to ";
        message += fn;
        logger().print(message,"")
      }
      */
    }
  }
  return 0;
}

//***********************************************************
bool RdmPriority::is_match(const char *line, const char *pattern) const {
  // returns true if there is a match
  // returns false if the pattern is not within the line

  int wildcard = 0;

  const char *last_pattern_start = 0;
  const char *last_line_start = 0;
  do {
    if (*pattern == *line) {
      if (wildcard == 1) {
        last_line_start = line + 1;
      }
      line++;
      pattern++;
      wildcard = 0;
    } else if (*pattern == '?') {
      if (*(line) == '\0') {  // the line is ended but char was expected
        return false;
      }
      if (wildcard == 1) {
        last_line_start = line + 1;
      }
      line++;
      pattern++;
      wildcard = 0;
    } else if (*pattern == '*') {
      if (*(pattern + 1) == '\0') {
        return true;
      }

      last_pattern_start = pattern;
      // last_line_start = line + 1;
      wildcard = 1;

      pattern++;
    } else if (wildcard) {
      if (*line == *pattern) {
        wildcard = 0;
        line++;
        pattern++;
        last_line_start = line + 1;
      } else {
        line++;
      }
    } else {
      if ((*pattern) == '\0' && (*line) == '\0') {  // end of mask
        return true;  // if the line also ends here then the pattern match
      } else {
        if (last_pattern_start != 0) {  // try to restart the mask on the rest
          pattern = last_pattern_start;
          line = last_line_start;
          last_line_start = 0;
        } else {
          return false;
        }
      }
    }

  } while (*line);

  if (*pattern == '\0') {
    return true;
  } else {
    return false;
  }
}
