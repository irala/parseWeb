#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>

using json = nlohmann::json;
using namespace std;

vector<string> determine_word(const string &line_to_words, const char word_separator, const set<char> &exclude_chars)
{
  bool exclude_char = false;
  stringstream current_word;
  vector<string> non_exclude_words;
  for (char i : line_to_words)
  {
    if (i == word_separator)
    {
      if (current_word.str().size() >= 1)
      {
        non_exclude_words.push_back(current_word.str());
        current_word.str("");
      }
      exclude_char = false;
      continue;
    }
    if (exclude_char)
    {
      continue;
    }
    if (exclude_chars.find(i) != exclude_chars.end())
    {
      current_word.str("");
      exclude_char = true;
      continue;
    }

    current_word << i;
  }
  if (current_word.str().size() >= 1)
  {
    non_exclude_words.push_back(current_word.str());
  }
  return non_exclude_words;
}

void test_line_to_words()
{
  auto result = determine_word("Car/los Bimito es gorda> guapa", ' ', {'<', '>', '/'});
  for (auto &&i : result)
  {
    cout << i << ' ';
  }
  cout << endl;
}

//algorithm that delimite words and save by lexer.json
void line_to_words_statistics(
    const string &line_to_words,
    const char word_separator,
    const set<string> &tags_to_save,
    const set<char> &excluded_chars,
    const int current_line,
    map<string, tuple<int, set<int>>> &map_word_statistics)
{

  
}

//calculate statistics for the words of lexer.tags_to_map
int main()
{
  test_line_to_words();
  std::ifstream i("lex.json");
  json jlex;
  // i >> jlex;
  // //C++17 structured bindings
  //  for (const auto &[k, v] : jlex.items())
  //  {
  //    cout << k << " : " << v.dump(4) << endl;
  //  }

  const auto &match = jlex.find("tags_to_map");
  const auto &ignore_match = jlex.find("chars_to_ignore");
  set<string> tags;
  set<char> ignore_chars;
  if (match != jlex.end())
  {
    //hay cosas
    tags = match.value().get<set<string>>();
  }

  if (ignore_match != jlex.end())
  {
    //hay cosas
    ignore_chars = ignore_match.value().get<set<char>>();
  }

  //dictionary k: tag v: tuple <repetitions,lines_present>
  map<string, tuple<int, set<int>>> map_word_statistics;

  std::ifstream ifs("html", std::ifstream::in);
  vector<string> lines;
  string str;
  while (getline(ifs, str))
  {
    lines.push_back(str);
  }
  ifs.close();

  for (size_t i = 0; i < lines.size(); i++)
  {
    /* code */
    line_to_words_statistics(lines[i], ' ', tags, ignore_chars, i, map_word_statistics);
  }

  return 0;
}