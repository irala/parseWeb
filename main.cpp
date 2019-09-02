#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>

using json = nlohmann::json;
using namespace std;

using tag_from_json_t = string;
using repetitions_t = int;
using lines_t = set<int>;
using collection_t = tuple<repetitions_t, lines_t>;
using map_tag_tuple_repetitions_lines_present_t = map<tag_from_json_t, collection_t>;

vector<string> determine_word(const string &line_to_words, const char word_separator, const set<char> &exclude_chars)
{
  bool exclude_char = false;
  stringstream current_word;
  vector<string> non_exclude_words;
  for (char i : line_to_words)
  {
    if (i == word_separator || exclude_chars.find(i) != exclude_chars.end())
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
    map_tag_tuple_repetitions_lines_present_t &map_word_statistics)
{
  auto result = determine_word(line_to_words, word_separator, excluded_chars);
  for (string current_word : result)
  {
    if (tags_to_save.find(current_word) != tags_to_save.end())
    {
      //current_word is any of the tags_to_save
      auto &[repetitions, lines] = map_word_statistics[current_word];
      repetitions++;
      lines.insert(current_line);
      map_word_statistics[current_word] = make_tuple(repetitions, lines);
    }
  }
}

vector<string> parse_commands(int argc, char *argv[])
{
  vector<string> commands;
  for (size_t i = 1; i < static_cast<size_t>(argc); i++)
  {
    /* code */
    commands.push_back(string(argv[i]));
  }
  return commands;
}

//calculate statistics for the words of lexer.tags_to_map
int main(int argc, char *argv[])
{

  vector<string> commands = parse_commands(argc, argv);
  for (auto &&i : commands)
  {
    cout << i << endl;
  }

  //test_line_to_words();
  std::ifstream i("lex.json");
  json jlex;
  i >> jlex;
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
    string ignore_chars_ = ignore_match.value().get<string>();
    for_each(ignore_chars_.begin(), ignore_chars_.end(), [&ignore_chars](char c) {
      ignore_chars.insert(c);
    });
  }

  //dictionary k: tag v: tuple <repetitions,lines_present>
  map_tag_tuple_repetitions_lines_present_t map_word_statistics;

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
  const auto &_match = map_word_statistics.find(commands[0]);
  if (_match == map_word_statistics.cend())
  {
    cout << commands[0] << " not exists in tags" << endl;
    return 1;
  }

  string _tag = _match->first;
  auto &[repetitions_, lines_] = _match->second;
  
  for (auto &&i : commands)
  {
    if (i == "repetitions")
    {
      cout <<_tag <<" : "<<repetitions_<<endl;
    }
    else if (i == "lines")
    {
      cout <<_tag <<" : ";
      for (auto &&line : lines_)
      {
        cout<<"-" <<line;
      }
      cout<<endl;
    }
    else
    {
      cout << "You got wrong the stats" << endl;
    }
  }

  // for (auto &[k, v] : map_word_statistics)
  // {
  //   // cout << k << " -> ";
  //   auto &[repetitions, lines] = v;
  //   stringstream s_lines;
  //   for (auto &&i : lines)
  //   {
  //     s_lines << ' ' << i;
  //   }
  //   // cout << repetitions << " : " << s_lines.str() << endl << endl ;
  // }

  return 0;
}
// read by commands tag , repetitions, lines or both.