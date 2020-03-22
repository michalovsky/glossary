#include "DefaultDescriptionParser.h"

#include "boost/algorithm/string.hpp"

namespace wordDescriptionDownloader
{
namespace
{
template <class Container>
void removeDuplications(Container& container)
{
    std::sort(container.begin(), container.end());
    container.erase(std::unique(container.begin(), container.end()), container.end());
}

void removeMarks(wordDescriptionRepository::Description&);
void removeDuplicationsInDescription(wordDescriptionRepository::Description&);
void removeDefinitionMarks(wordDescriptionRepository::Definition&);
void removeExampleMarks(wordDescriptionRepository::Example&);
void removeSentenceMarks(wordDescriptionRepository::Sentence&);
bool isDefinition(const std::string&);
bool isExample(const std::string&);
bool isSentence(const std::string&);
void trimEmptySpaces(std::string&);

const std::string definitionPrefix{": "};
const std::string examplePrefix{"// "};
const std::string sentencePrefix{"; "};
}

boost::optional<wordDescriptionRepository::Description> DefaultDescriptionParser::parse(const std::vector<std::string>& lines) const
{
    wordDescriptionRepository::Description description;
    bool previousLineIsDefinition = false;

    wordDescriptionRepository::Definition definition;

    for (const auto& line : lines)
    {
        if (previousLineIsDefinition && !isExample(line))
        {
            description.definitionsWithExamples.push_back({definition, boost::none});
            previousLineIsDefinition = false;
        }

        if (isSentence(line))
        {
            description.sentences.push_back(line);
        }

        if (previousLineIsDefinition && isExample(line))
        {
            description.definitionsWithExamples.push_back({definition, line});
            previousLineIsDefinition = false;
        }

        if (isDefinition(line))
        {
            definition = line;
            previousLineIsDefinition = true;
            if (line == lines.back())
            {
                description.definitionsWithExamples.push_back({definition, boost::none});
            }
        }
    }

    if (description.empty())
    {
        return boost::none;
    }

    removeMarks(description);
    removeDuplicationsInDescription(description);
    return description;
}

namespace
{
void removeMarks(wordDescriptionRepository::Description& description)
{
    for (auto& definitionAndExample : description.definitionsWithExamples)
    {
        removeDefinitionMarks(definitionAndExample.definition);
        if (definitionAndExample.example)
        {
            removeExampleMarks(*definitionAndExample.example);
        }
    }

    for (auto& sentence : description.sentences)
    {
        removeSentenceMarks(sentence);
    }
}

void removeDuplicationsInDescription(wordDescriptionRepository::Description& description)
{
    auto& definitionsWithExamples = description.definitionsWithExamples;
    auto& sentences = description.sentences;
    removeDuplications(definitionsWithExamples);
    removeDuplications(sentences);
}

void removeDefinitionMarks(wordDescriptionRepository::Definition& definition)
{
    definition.erase(0, definitionPrefix.size());
    trimEmptySpaces(definition);
}

void removeExampleMarks(wordDescriptionRepository::Example& example)
{
    example.erase(0, examplePrefix.size());
    trimEmptySpaces(example);
}

void removeSentenceMarks(wordDescriptionRepository::Sentence& sentence)
{
    sentence.erase(0, sentencePrefix.size());
    trimEmptySpaces(sentence);
}

bool isDefinition(const std::string& line)
{
    return line.rfind(definitionPrefix, 0) != std::string::npos;
}

bool isExample(const std::string& line)
{
    return line.rfind(examplePrefix, 0) != std::string::npos;
}

bool isSentence(const std::string& line)
{
    return line.rfind(sentencePrefix, 0) != std::string::npos;
}

void trimEmptySpaces(std::string& line)
{
    boost::algorithm::trim(line);
}

}
}