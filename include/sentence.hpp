#pragma once
#include "tool_kit.hpp"

class Sentence 
{
public:
    Sentence();
    Sentence(std::string value);

    std::string  getQuestionTag() const;
    std::string  getWarnings() const;
    std::string  getExplanation() const;
    std::string  createQuestionTags( std::string );

private:
    std::string  _subject;
    std::string  _verb;
    std::string  _questionTag;
    std::string  _warnings;
    std::string  _explanation = "";
    std::vector  <std::string> _splittedContent;
  
    std::string  findVerb();
    std::string  findSubject();
    std::string  generateQuestionTag();
    bool         theSentenceIsAOrder() const;
    bool         theSentenceIsNegative() const;
    void         setQuestionTag( std::string questionTag );
};
