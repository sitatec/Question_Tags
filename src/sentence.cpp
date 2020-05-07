#include "include/sentence.hpp"

using std::string ;
using namespace std::literals;



Sentence::Sentence(){}



/**
 * @brief Constructor of Sentence class.
 * 
 * @param[out] value The sentence whose the question tag will be generated.
 * @exception The `Error` exception class will be throw with the type `Error::EMPTY_SENTENCE`
 * if the parameter is empty.
 */
Sentence::Sentence (string value)
{
    tool::loadDictionaries();
    auto questionTag = createQuestionTags(value);
    setQuestionTag( questionTag );
}



std::string Sentence::createQuestionTags (std::string sentence)
{
    _verb.clear();
    _subject.clear();
    _splittedContent.clear();

    if( sentence.empty() ) throw Error( Error::EMPTY_SENTENCE, "Error the sentence can't be empty"s);
    auto lowedSentence  = tool::toLowerCase( sentence );
    auto sentenceWords  = tool::split( lowedSentence );

    for(auto word : sentenceWords){
        auto trimedWord = tool::trim( word );

        if( ispunct( *(trimedWord.end() - ONE_CHAR) )) {
            trimedWord = string{ trimedWord.begin(), trimedWord.end() - ONE_CHAR };
        }

        _splittedContent.push_back( trimedWord );
    }

    return generateQuestionTag();
}



/**
 * @brief Check if the sentence is an order or imperative.
 * @details Check if the first word is a verb or never...
 * 
 * @return `true` If the sentence is an order else `false`.
 */
bool Sentence::theSentenceIsAOrder() const
{
    auto firstWord = _splittedContent.front();
    return ( firstWord == "never" ||   firstWord == "don't" ||
           (firstWord != _subject && tool::isVerb( firstWord)) );
}



/**
 * @brief Set the value of the question tag attribut.
 * 
 * @param[in] questionTag the question tag.
 * 
 * @pre The parameter that it receive shouldn't be empty.
 * 
 * @exception The `Error` exception class will be throw with the type `Error::EMPTY_QUESTION_TAG`
 * if the parameter is empty.
 */
void Sentence::setQuestionTag( string questionTag )
{   
    if( questionTag.empty() )
        throw Error(Error::EMPTY_QUESTION_TAG, "Error the question tag is empty please check that your"
                                               " sentence is in the form<i style=\"color: green ;\">"
                                               " \" Subject + Verb + Complement \" </i> and try again."s);
    else _questionTag = questionTag;
}



/**
 * @brief Check if the sentence is negative.
 * @details Check if the sentence has negative word like *few* or *little*.
 * 
 * @return `true` If the sentence is negative else `false`.
 */
bool Sentence::theSentenceIsNegative() const
{
    for(int i = 0, j = _splittedContent.size(); i < j; i++ ){

        auto word = _splittedContent[ i ];
        auto previousWord = _splittedContent[ i - 1 ];
        // If the negative words "little" and "few" are preceded by "a"
        // they aren't considered negative word
        if( word == "little" || word == "few"){
            if( previousWord == "a" ) return false;
        }
        if( tool::isNegativeWord( word )) return true ;
    } return false;
}



/**
 * @brief Create the corresponding question tag of the sentence.
 * @details Calls many function like `theSentenceIsAOrder()`,`findSubject()`or `findVerb` and
 * others functions of `tool` namespace for generate the question tag by check if sentence
 * is a order or imperative, if the sentence is negative and check verb tense...
 * If the sentence starts by *let's* the question tag will be *shall we*
 * If the sentence starts by a verb the question tag will be *will you* or *can you* or others
 * depending of sentence.
 * 
 * @return The question tag corresponding to the sentence if all the attributes found else it
 * returns void string.
 */
string Sentence::generateQuestionTag()
{
    auto firstWord = _splittedContent.front();
    if( firstWord == "let's"s){
        _explanation = "A suggestion using let's can be tagged with <i style=\"color: green;\">"
                       "\"shall we ?\"</i>"s;
        return string("shall we ? "s);
    }

    if( theSentenceIsAOrder() ){
        if(_splittedContent.front() == "help") {
            _explanation = "Imperatives are often tagged with will / won't / would / can / can'"
                           " / could + you?"s;
            return ("can you ? "s);
        }
        _explanation = "Imperatives are often tagged with will / won't / would / can / can't / "
                       "could + you?"s;
        return string("will you ? "s);
    }

    auto subject = findSubject();
    if( subject == "i" ) subject = "I";
        _subject = subject;

    auto verb = findVerb();
    _verb = verb;

    if( _subject.empty() ){
        if( tool::conjugatedInThirdPersonSingular(_verb) ){
            _subject = string("he | she | it"s);
            _warnings += " You are using a beta version of Question tag. Proper names, "
                         "common names and nominal group are not completely supported on "
                         "this version, if the software does not recognize the subject "
                         "and that the verb is conjugated in the third person singular "
                         "it displays <span style=\"color: orange\">\"he | she | it \""
                         "</span>.Please replace the subject with a personal pronoun to fix that"s;
        }
        else {
            // Find the subject for the sentences like: Few "of us" care about writing clean code
            int ofPosInList, usPosInList;
            if( tool::isInList( &_splittedContent, "of", &ofPosInList ) &&
                tool::isInList( &_splittedContent, "us", &usPosInList ) ){
                if( ofPosInList < usPosInList ) _subject = "we";
            }
            if( _subject.empty() ) {
                _subject = "they"s;
                _warnings += "You are using a beta version of Question tag. Proper names, "
                             "common names and nominal group are not completely supported on "
                             "this version, if the subject of the tag question does not "
                             "correspond to the subject of your sentence, please replace "
                             "the subject with a personal pronoun and try again.";
            }
        } 
    }

    bool questionTagVerbIsNegative = ( _verb.find('\'') != _verb.npos );
    string affirmativeVerb{""s};
    if( theSentenceIsNegative() ){
        _explanation += " Sentence that content a negative word lead to an affirmative tag question"s;
        if( questionTagVerbIsNegative ){
            if(_verb == "can't") affirmativeVerb = "can";
            // The following instruction can't appliqued to "can"
            // because "can" end by "n" else the result will be "ca"
            else affirmativeVerb = string( _verb.begin(), _verb.end() - THREE_CHARS );

            _verb = affirmativeVerb ;
        }
    }

    if(!_verb.empty() ){
        auto questionTag = _verb + SPACE + _subject + " ? "s;
        return questionTag;
    }
    return VOID_STRING;
}



/**
 * @brief Returns the question tag of the sentence.
 * 
 * @return The question tag of the sentence.
 * 
 * @exception The `Error` exception class will be throw with the type `Error::EMPTY_QUESTION_TAG`
 * if attribut `_questionTag` is empty.
 */
string Sentence::getQuestionTag() const
{   
    if( _questionTag.empty() )
        throw Error( Error::EMPTY_QUESTION_TAG, "Error the question tag is empty please check that your"
                                                " sentence is in the form<i style=\"color: green ;\">"
                                                " \" Subject + Verb + Complement \" </i> and try again." );
    return _questionTag;
}



/**
 * @brief Find the subjet of sentence
 * 
 * @return The subject if it foud else returns void string 
 */
string Sentence::findSubject() 
{
    for (int i = 0, j = _splittedContent.size(); i < j ; i++) {

        int nextI{ i + 1 };
        string nextWord{""};
        string word = _splittedContent[  i  ];

        if( nextI < j )
            nextWord = _splittedContent[ nextI ];     

        auto apostrophe = std::find( word.begin(), word.end(), '\'' );
        string wordBeforeApostrophe{ word.begin(), apostrophe};
        if( apostrophe != word.end() ) word = wordBeforeApostrophe;

        if (tool::isPersonalPronoun( word )) return word;
        else if (tool::isIndefinitePluralPronoun( word )) return "they"s;
        else if (tool::isIndefiniteSingularPronoun( word )) return "it"s;

        if (word == "there"s) return "there"s ;

        if ( word == "no"s || word == "some"s || word == "every"s ){
            if( nextWord == "one"s || nextWord == "body"s ) return "they"s;
        }
    }
    return VOID_STRING;
}



/**
 * @brief Find the verb of sentence
 * 
 * @return The verb if it found else returns void string
 * 
 * @exception The `Error` exception class will be throw with the type `Error::VERB_NOT_FOUND`
 * if no verb found in the senten
 */
string Sentence::findVerb () 
{
    for (int i = 0, j = _splittedContent.size(); i < j ; i++) { 

        string nextWord{VOID_STRING}, nextNextWord{VOID_STRING};
        int positionInDictionary{0}, nextI{ i + 1 }, nextNextI{ i + 2 };
        auto word = _splittedContent[i];

        if( nextI < j )     nextWord     = _splittedContent[   nextI   ];
        if( nextNextI < j ) nextNextWord = _splittedContent[ nextNextI ];

        if( word == "cannot" ) {
            word = "can";
            nextWord = "not";
        }

        if( word == _subject ) continue;

        if (tool::isModal(word, &positionInDictionary)) {
            if( nextWord == "not"s )
                return word;
            return tool::toggleAffOrNeg(*tool::getModalVerbsList(), positionInDictionary );
        }

        else if(tool::isAuxiliary(word, &positionInDictionary)) {
            if( nextWord == "not"s )
                return word;
            if( nextWord == "to"s) {// Exception for the verbs like "I have to work"
                if( word == "have"s || word == "has"s) return "don't"s;
                if( word == "had"s ) return "didn't"s;
            }
            return tool::toggleAffOrNeg(*tool::getAuxiliariesList(), positionInDictionary );
        }

        auto apostrophe = std::find( word.begin(), word.end(), '\'' );
        string negativeContraction{VOID_STRING};
        if( apostrophe != word.end() ){
            if(  word.length() > 3 ) 
                negativeContraction = string{ apostrophe - ONE_CHAR, word.end() };
            if( negativeContraction == "n't"s ){
                word = string( word.begin(), apostrophe - ONE_CHAR );
                nextWord = "not"s;
            }
            else{
                auto contractedForm = tool::checkContractedForm( word, nextWord, nextNextWord );
                if( !contractedForm.empty() ) return contractedForm;
            } 
        }

        if (tool::isInThePresent( word )) {

            if( tool::conjugatedInThirdPersonSingular( word ) ){

                for( int k = 0; k < i; k++ ){
                    if( tool::isThirdPersonSingular( _splittedContent[ k ] ))
                        return ( nextWord == "not"s ) ? "does"s : "doesn't"s;
                }
            }
            return ( nextWord == "not"s ) ? "do"s : "don't"s;
        }
        else if (tool::isIrregularPreterit( word ) || tool::isRegularPreterit( word )) {
            return ( nextWord == "not"s ) ? "did"s : "didn't"s;
        }
    }
    throw Error( Error::VERB_NOT_FOUND, "The software did not find or recognized the verb of the "
                                        "sentence. Please check that your sentence is in the form"
                                        "<i style=\"color: green ;\">\" Subject + Verb + Complement"
                                        " \" </i> and try again."s );
    return string("");
}   



string Sentence::getWarnings() const
{
    return _warnings;
}



string Sentence::getExplanation() const
{
    return _explanation;
}

