#include "tiktok.h"

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <queue>

/**
 * Check if file can successfully be opened
 * 
 * @param fileName name of file to be opened
 *      
 * @return the opened file stream
 */
template<typename T>
T validateFile(const std::string &fileName);
std::vector<std::string> parseHashtag(std::string text);
void parseJSON(std::ifstream &input, 
               std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map, 
               const std::string &mode);

int main(int argc, char const *argv[])
{
    if(argc != 4)
    {
        std::cerr << "Invalid Argument Count" << std::endl;
        exit(1);
    }

    std::ifstream input = validateFile<std::ifstream>(argv[1]);
    std::ofstream output = validateFile<std::ofstream>(argv[2]);
    std::string mode = argv[3];
    std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > map;
    parseJSON(input, map, mode);

    if(mode == "hashtag")
    {
        std::priority_queue<std::pair<std::string, std::pair<unsigned long long, unsigned long long> >, 
                            std::vector<std::pair<std::string, std::pair<unsigned long long, unsigned long long> > >,
                            compareHashtag> topHashtags;
        for(const std::pair<const std::string, std::pair<std::priority_queue<TikTok>, unsigned long long>> &hashtagEntry : map) 
        {
            std::pair<std::string, std::pair<unsigned long long, unsigned long long>> hashtagData = {hashtagEntry.first,
                                                                                        {hashtagEntry.second.first.size(), hashtagEntry.second.second}};
            topHashtags.push(hashtagData);
        }
        std::vector<std::string> top10Hashtags;
        for(int x = 0; x < 10 && !topHashtags.empty(); x++)
        {
            top10Hashtags.push_back(topHashtags.top().first);
            topHashtags.pop();
        }

        printTopHashtags(output, top10Hashtags, map);
    }
    else if(mode == "sound")
    {
        std::priority_queue<std::pair<std::string, unsigned long long>,
                            std::vector<std::pair<std::string, unsigned long long> >,
                            compareSound> topSounds;
        for(const std::pair<const std::string, std::pair<std::priority_queue<TikTok>, unsigned long long>> &soundEntry : map)
        {
            std::pair<std::string, unsigned long long> soundData = {soundEntry.first, soundEntry.second.second};
            topSounds.push(soundData);
        }

        std::vector<std::string> top10Sounds;
        for(int x = 0; x < 10 && !topSounds.empty(); x++)
        {
            top10Sounds.push_back(topSounds.top().first);
            topSounds.pop();
        }
        printTopSounds(output, top10Sounds, map);
    }

    return 0;
}

template<typename T>
T validateFile(const std::string &fileName)
{
    T stream(fileName);
    if(!stream.good()) std::cerr << "File Error" << std::endl;
    return stream;
}
std::vector<std::string> parseHashtag(std::string text)
{
    std::vector<std::string> output;

    // define a regular expression to match hashtags
    std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)");

    // create an iterator for matching
    std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
    std::sregex_iterator endIterator;

    // iterate over the matches and extract the hashtags
    while (hashtagIterator != endIterator) 
    {
        std::smatch match = *hashtagIterator;
        std::string hashtag = match.str(1);  // extract the first capturing group

        output.push_back(hashtag);

        ++hashtagIterator;
    }
    return output;
}
void parseJSON(std::ifstream &input, 
               std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map, 
               const std::string &mode)
{
    std::string json_content, line;
    while(std::getline(input, line)) 
    {
        std::string ID, text, coverURL, webURL, soundName, soundAuthor, soundID;
        unsigned long long viewCount;

        size_t IDTagEnd = line.find("\"id\": ") + 6;
        size_t IDStart = line.find_first_not_of("\"", IDTagEnd);
        size_t IDEnd = line.find("\"", IDStart);
        ID = line.substr(IDStart, IDEnd - IDStart);

        size_t textTagEnd = line.find("\"text\": ") + 8;
        size_t textStart = line.find_first_not_of("\"", textTagEnd);
        size_t textEnd = line.find("\"", textStart);
        text = line.substr(textStart, textEnd - textStart);

        size_t coverURLTagEnd = line.find("\"coverUrl\": ") + 12;
        size_t coverURLStart = line.find_first_not_of("\"", coverURLTagEnd);
        size_t coverURLEnd = line.find("\"", coverURLStart);
        coverURL = line.substr(coverURLStart, coverURLEnd - coverURLStart);

        size_t webURLTagEnd = line.find("\"webVideoUrl\": ") + 15;
        size_t webURLStart = line.find_first_not_of("\"", webURLTagEnd);
        size_t webURLEnd = line.find("\"", webURLStart);
        webURL = line.substr(webURLStart, webURLEnd - webURLStart);

        size_t soundNameTagEnd = line.find("\"musicName\": ") + 13;
        size_t soundNameStart = line.find_first_not_of("\"", soundNameTagEnd);
        size_t soundNameEnd = line.find("\"", soundNameStart);
        soundName = line.substr(soundNameStart, soundNameEnd - soundNameStart);

        size_t soundAuthorTagEnd = line.find("\"musicAuthor\": ") + 15;
        size_t soundAuthorStart = line.find_first_not_of("\"", soundAuthorTagEnd);
        size_t soundAuthorEnd = line.find("\"", soundAuthorStart);
        soundAuthor = line.substr(soundAuthorStart, soundAuthorEnd - soundAuthorStart);

        size_t soundIDTagEnd = line.find("\"musicId\": ") + 11;
        size_t soundIDStart = line.find_first_not_of("\"", soundIDTagEnd);
        size_t soundIDEnd = line.find("\"", soundIDStart);
        soundID = line.substr(soundIDStart, soundIDEnd - soundIDStart);

        size_t viewCountTagEnd = line.find("\"playCount\": ") + 13;
        size_t viewCountStart = line.find_first_not_of("\"", viewCountTagEnd);
        size_t viewCountEnd = line.find("\"", viewCountStart);
        viewCount = std::stoi(line.substr(viewCountStart, viewCountEnd - viewCountStart));
       
        TikTok video(ID, text, coverURL, webURL, soundName, soundAuthor, soundID, viewCount);
        if(mode == "hashtag")
        {
            std::vector<std::string> hashtags = parseHashtag(text);
            for(std::string hashtag : hashtags)
            {
                map[hashtag].first.push(video);   
                map[hashtag].second += viewCount;
            }
        }
        else if(mode == "sound")
        {
            map[soundID].first.push(video);
            map[soundID].second += viewCount; 
        }      
    }
}