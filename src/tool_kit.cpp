#include "include/tool_kit.hpp"
#include <regex>
#include <cctype>
#include <QFile>
#include <QTextStream>
#include "include/sentence.hpp"

using std::string ;
using std::vector ;
using namespace std::literals; 


static const char* DICTIONARIES_PATH = ":/ressources/dictionaries/";
static const int   INDEX_OF_LAST_VERB_ENDING_BY_Y = 64;
static const int   INDEX_OF_LAST_VERB_ENDING_BY_S_X_SH_CH = 190;
  
std::vector   <std::string>   preteritVerbs;
std::vector   <std::string>   infinitiveVerbs;
std::vector   <std::string>   auxiliariesList;
std::vector   <std::string>   modalVerbsList;
std::vector   <std::string>   pastParticiples;

static std::unordered_map <tool::Dictionary, vector <string>*> dictionaries{
        { tool::Dictionary::PRETERIT,        &preteritVerbs   },
        { tool::Dictionary::INFINITIVE,      &infinitiveVerbs },
        { tool::Dictionary::AUXILIARIES,     &auxiliariesList },
        { tool::Dictionary::MODAL_VERBS,     &modalVerbsList  },
        { tool::Dictionary::PAST_PARTICIPLE, &pastParticiples }
    };

/**
 * @brief Contents the geters code for don't duplicate
 * 
 * @param list 
 * @param dictionary 
 * @return A `std::vector <string>`depending to the calling getter
 */
std::vector <string>* tool::getersTemplate(std::vector <string>* list, Dictionary dictionary)
{
    if( list->size() == 0 )
        list = dictionaries[ dictionary ];
    return list;
}



/**
 * @brief Get the preterit verbs in the `tool::Dictionary::PRETERIT`
 * 
 * @return `std::vector <string>` 
 */
std::vector <string>* tool::getPreteritVerbs()
{
    return getersTemplate( &preteritVerbs, Dictionary::PRETERIT);
}



/**
 * @brief Get the infinitive verbs in the `tool:Dictionary::PRETERIT`
 * 
 * @return `std::vector <string>` 
 */
std::vector <string>* tool::getInfinitiveVerbs()
{
    return getersTemplate( &infinitiveVerbs, Dictionary::INFINITIVE);
}



/**
 * @brief Get the auxiliaries verbs in the `tool::Dictionary::AUXILIARIES`
 * 
 * @return `std::vector <string>` 
 */
std::vector <string>* tool::getAuxiliariesList()
{
    return getersTemplate( &auxiliariesList, Dictionary::AUXILIARIES);
}



/**
 * @brief Get the modal verbs in the `tool::Dictionary::AUXILIARIES`
 * 
 * @return `std::vector <string>` 
 */
std::vector <string>* tool::getModalVerbsList()
{
    return getersTemplate( &modalVerbsList, Dictionary::MODAL_VERBS);
}



/**
 * @brief Checks if the word that it receives as a parameter is a modal verb.
 * @details Calls the `checkVerbState` function and pass `tool::Dictionary::MODAL_VERBS`,
 * the word to check and a pointer in parameter.
 * 
 * @param word The word to check
 * @param positionInDictionary  An int  pointer for writing the position of the entity(string) in
 * the dictionary so that the calling function will knows the position of the entity, but the
 * poiter value will not assigned in this function it will be passed in parameter to an other function.
 * 
 * @return `true` if the word is a modal verb else false
 */
bool tool::isModal (std::string word, int* positionInDictionary) 
{   
    return checkVerbState( &modalVerbsList, word, Dictionary::MODAL_VERBS, positionInDictionary );
}



/**
 * @brief Checks if the word that it receives as a parameter is an auxiliary verb.
 * @details Calls the `checkVerbState` function and pass `tool::Dictionary::AUXILIARIES`,
 * the word to check and a pointer in parameter.
 * 
 * @param word The word to check
 * @param positionInDictionary  An int  pointer for writing the position of the entity(string) in
 * the dictionary so that the calling function will knows the position of the entity, but the
 * poiter value will not assigned in this function it will be passed in parameter to an other function.
 * 
 * @return `true` if the word is a auxiliary verb else false
 */
bool tool::isAuxiliary (std::string word, int* positionInDictionary ) 
{    
    return checkVerbState( &auxiliariesList, word, Dictionary::AUXILIARIES, positionInDictionary );
}


/**
 * @brief Searchs a entity in a list
 * @details Thi function search a string variable in a vector that it receives as a parameter
 * 
 * @param list A `vector <string>` where the function will find the `string`
 * @param entity String to find
 * @param positionInList An int  pointer for writing the position of the entity(string) in
 * the list(vector) so that the calling function will knows the position of the entity.
 * 
 * @return `true` if the entity found else `false`
 */
bool tool::isInList( std::vector <string>* list , std::string const& entity,
                                            int* positionInList) 
{
    if( list->empty() || entity.empty() ) return false;

    auto startOfList = std::begin( *list );
    auto endOfList = std::end( *list );
    auto itemPositionFound = std::find (startOfList, endOfList, entity );

    if( itemPositionFound != endOfList ){
        if(  positionInList != nullptr  )
            *positionInList = (itemPositionFound - startOfList) + 1 ;
        return true;
    }
    return false;
}


/**
 * @brief Check if a verb is modal, auxiliary or simple verb
 * @details 
 * 
 * @param list 
 * @param entity 
 * @param dictionary 
 * @param positionInList 
 * 
 * @return true 
 * @return false 
 */
bool tool::checkVerbState( std::vector <std::string>* list, std::string entity,
                    Dictionary dictionary, int* positionInList)
{    
    if(   list->size() == 0 ) loadDictionary( dictionary ) ;
    return tool::isInList( list, entity, positionInList );
}



/**
 * @brief Check if the a verb is in the present tense.
 * @details Look for the verb that it receive as a parameter in the infinitive dictionary 
 * `tool::Dictionary::INFINITIVE` if the verb is not found it will check if the verb is conjugated
 *  at the third person singular by converting the verb to infinitive and search the verb again
 *  in the infinitive dictionary.
 * 
 * @param verb Verb to check.
 * 
 * @return `true` If the verb found in dictionary else returns `false`.
 */
bool tool::isInThePresent (string verb) 
{    
    bool isInPresent = checkVerbState( &infinitiveVerbs, verb, Dictionary::INFINITIVE );
    if( isInPresent )
        return true;   
    string unconjugatedVerb = toUnconjugated( verb );
    if( !unconjugatedVerb.empty() && unconjugatedVerb != VOID_STRING)
        return true;
    else return false;
}


/**
 * @brief Check if a verb is in the preterit tense.
 * @details search the verb in the preterit dictionary `tool::Dictionary::PRETERIT`.
 * 
 * @param verb Verb to check.
 * 
 * @return `true`
 */
bool tool::isIrregularPreterit (string verb)
{   
    return checkVerbState(  &preteritVerbs, verb, Dictionary::PRETERIT);
}



bool tool::isRegularPreterit (string verb)
{
    if( verb.empty() || verb.length() < 4 ) return false;
    verb = tool::preteritToPresent(verb);
    if( verb.empty() ) return false;
    return true;
}



bool tool::isVerb (string verb)
{
    if( ispunct( *(verb.end() - ONE_CHAR) )) {
        verb = string{ verb.begin(), verb.end() - ONE_CHAR };
    }
    return ( tool::isInThePresent(verb) || tool::isIrregularPreterit(verb) || 
             tool::isRegularPreterit(verb) );
}



string tool::preteritToPresent (string verb)
{
    auto startOfVerb = std::begin( verb ), endOfVerb = std::end( verb );

    if( tool::hasDoubleConsonantInPreterit( verb )){
        string infintiveOFVerb{ startOfVerb, endOfVerb - THREE_CHARS };
        if( tool::isInList( &infinitiveVerbs, infintiveOFVerb))
            return infintiveOFVerb ;
    }

    for( auto verbInDict : infinitiveVerbs ){
        if( tool::presentToPreterit( verbInDict ) == verb ) return verbInDict;
    }
    return VOID_STRING;
}



string tool::presentToPreterit (string verb)
{
    auto startOfVerb = std::begin( verb ), endOfVerb = std::end( verb );
    bool beforLastIsVowel = tool::isVowel( *(endOfVerb - TWO_CHARS) );

    if( *(endOfVerb - ONE_CHAR) == 'y' && !beforLastIsVowel)
        return string( startOfVerb, endOfVerb - ONE_CHAR ).append("ied"s);
    if( *(endOfVerb - ONE_CHAR) == 'e' )
        return verb.append("d"s);
    else 
        return verb.append("ed"s);
}



string tool::toggleTense(string verb) 
{
    string lastTwoChars( std::end(verb) - TWO_CHARS, std::end(verb) );
    if( lastTwoChars == "ed"s )
        return preteritToPresent( verb );
    else
        return presentToPreterit( verb );
}



string tool::toggleAffOrNeg (vector <string> dictionary, int positionInDictionary) 
{
    int const TWO_POSITIONS = 2;
    if( positionInDictionary % 2 ) // If positionInDictionary is pair
        return dictionary[ positionInDictionary ];
    else 
        return dictionary [ positionInDictionary - TWO_POSITIONS ];
}



bool tool::hasDoubleConsonantInPreterit(string verb)
{   
    if( verb.length() >= 5 ){
        bool hasDoubleConsonant =( *( verb.end() - FOUR_CHARS ) == *( verb.end() - THREE_CHARS ) );
        bool endBy_Ed =( *( verb.end() - ONE_CHAR ) == 'd' && *( verb.end() - TWO_CHARS ) == 'e' );
        return ( hasDoubleConsonant && endBy_Ed ) ;
    }
    else return false;
}



string tool::conjugateInThirdPerson(string verb)
{
    if( verb.length() < TWO_CHARS ) return VOID_STRING;
    auto lastChar = (std::end( verb ) - ONE_CHAR), startOfVerb = std::begin( verb );
    std::regex reg(".*[cs]h$|.*s$|.*x$|.*o$|.*z$"s); 

    if ( std::regex_match(verb, reg) )
         return verb.append("es"s);
    if ( verb.length() > TWO_CHARS && *lastChar == 'y' && !tool::isVowel( *(lastChar - ONE_CHAR) ))
        return string( startOfVerb, lastChar ).append("ies"s);
    else return verb.append("s"s);
}



string tool::toUnconjugated (string verb)
{
    std::regex endByIes(".*ies$");
    std::regex endByEs(".*es$");
    std::regex endByS(".*s$");

    auto toInfinitive = [ &verb ] (std::regex regex, int endIndex, string charsToRemove,
                                   string charsToAppend = "") -> string {
        if( std::regex_match( verb, regex ) ){
            auto iPosition = verb.find_last_of( charsToRemove );
            string unconjugatedVerb{ verb.begin(), verb.end() - charsToRemove.size() };
            unconjugatedVerb.append( charsToAppend );

            if( iPosition != verb.npos ) {
                for (int i = 0; i < endIndex ; i++ ) {
                    if( infinitiveVerbs.at( i ) == unconjugatedVerb ) return unconjugatedVerb;
                }
            }
        }
        return VOID_STRING;

    };

    auto unconjugatedVerb = toInfinitive( endByIes, INDEX_OF_LAST_VERB_ENDING_BY_Y, "ies"s , "y" );
    if( unconjugatedVerb != VOID_STRING ) return  unconjugatedVerb;

    unconjugatedVerb = toInfinitive( endByEs, INDEX_OF_LAST_VERB_ENDING_BY_S_X_SH_CH, "es"s );
    if( unconjugatedVerb != VOID_STRING ) return  unconjugatedVerb;

    unconjugatedVerb = toInfinitive( endByS, infinitiveVerbs.size(), "s"s );
    if( unconjugatedVerb != VOID_STRING ) return unconjugatedVerb;

    return VOID_STRING;
}



string tool::toLowerCase (string stringToLower) 
{
    std::transform (std::begin( stringToLower ), std::end( stringToLower ), std::begin( stringToLower ),
                                [] (char c) { 
                                    return tolower(c) ; 
                                });
    return stringToLower ;
}



string tool::ltrim (const string& stringToTrim)
{
	auto firstCaracter = stringToTrim.find_first_not_of( WHITESPACE );
	return (firstCaracter == string::npos) ? VOID_STRING : stringToTrim.substr( firstCaracter );
}



string tool::rtrim (const string& stringToTrim) 
{
	auto lastCaracter = stringToTrim.find_last_not_of( WHITESPACE );
	return (lastCaracter == string::npos) ? VOID_STRING : stringToTrim.substr(0, lastCaracter + 1);
}



string tool::trim (const string& stringToTrim) 
{
	return rtrim( ltrim(stringToTrim) );
}



bool tool::isVowel (char c){
    return ( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' );
}



bool tool::isPersonalPronoun (string str)
{
    return  (  str  == "i"s   ||  str  ==  "you"s ||    str ==   "he"s  || str == "she"s      ||
               str  == "it"s  ||  str  ==  "we"s  ||    str ==   "they"s);
}



bool tool::isIndefiniteSingularPronoun( std::string str ) 
{
    return ( str ==  "that"s  || str ==  "this"s  ||  str  == "nothing"s|| str =="everything"s||
             str =="something"|| str =="anything"s);
}
 


bool tool::isIndefinitePluralPronoun( std::string str )
{
    return ( str == "these"s  ||str  ==  "those"s ||  str ==  "nobody"s || str == "somebody"s || 
             str == "anybody"s||str  =="everybody"||  str == "someone"s || str == "anyone"s   || 
             str =="everyone"s||str  =="some-one"s||  str =="every-one"s|| str == "any-one"s  ||  
             str == "no-one"s ||str  == "none"s   );
}



bool tool::isNegativeWord (string str)
{
    return ( str == "barely"s || str == "hardly"s ||  str == "neither"s || str  ==  "no"s     ||
             str == "none"s   || str == "nothing"s||  str == "seldom"s  || str  ==  "never"s  ||
             str == "rarely"s || str == "little"s ||  str == "nobody"s  || str  == "scarcely"s||
             str == "few"s    || str == "no-one"s  );
}



bool tool::isThirdPersonSingular(string str){
    return  ( str  ==  "he"s  ||  str  == "she"s  || str == "it"s );
}



bool tool::conjugatedInThirdPersonSingular (string  verb)
{
    if( verb.length() >= 4 ){
        bool isNegativeForm = string( verb.end() - THREE_CHARS, verb.end() ) == "n't"s;
        if( isNegativeForm ) verb = string( verb.begin(), verb.end() - THREE_CHARS );
    }
    if( verb == "is"s || verb == "has"s || verb == "does"s || verb == "goes"s)
        return true;
    verb = tool::toUnconjugated( verb );
    if( verb.empty() ) return false;
    return true;   
}



std::unordered_map <tool::Dictionary, string> tool::getDictionariesPath()
{
    std::unordered_map <Dictionary, string> pathsList;

        pathsList = {
            { Dictionary::PRETERIT,        DICTIONARIES_PATH  +  "preterit_irregular.txt"s },
            { Dictionary::INFINITIVE,      DICTIONARIES_PATH  +  "infinitive_verbs.txt"s },
            { Dictionary::AUXILIARIES,     DICTIONARIES_PATH  +  "auxiliaries.txt"s },
            { Dictionary::MODAL_VERBS,     DICTIONARIES_PATH  +  "modal_verbs.txt"s },
            { Dictionary::PAST_PARTICIPLE, DICTIONARIES_PATH  +  "past_participles.txt"s }
        };
    return pathsList;
}



void tool::loadDictionary (Dictionary dictionaryName)
{
    auto pathsList = tool::getDictionariesPath();
    auto dictionaryPath = pathsList[ dictionaryName ];
    QFile dictionary ( dictionaryPath.c_str() );

    if( dictionary.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        QTextStream in( &dictionary );
        while ( !in.atEnd() ) {
            dictionaries[dictionaryName]->push_back( in.readLine().toStdString() ) ;
        }
    }
    else
        throw Error(Error::DICTIONARY_CAN_BE_OPEN, "Error loading dictionaries. Please"
                                                   "try again or restart the software."s);
}



void tool::loadDictionaries()
{
    for(auto dictionary : dictionaries)
    {
        loadDictionary( dictionary.first );
    }
}


/*
std::unordered_map < string, vector <string> > tool::generateAQuiz()
{
    std::unordered_map < string, vector <string> > quizzes;
    QFile file(":/ressources/dictionaries/quiz.txt");
    if( file.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        Sentence sentence;
        int const NUMBER_OF_ANSWERS = 3;
        vector <string> quizAnswers( NUMBER_OF_ANSWERS );
        int trueAnswerPosition = rand() % NUMBER_OF_ANSWERS;
        for( int i = 0; i < 10; i++ ){
            auto line = file.readLine().toStdString();
            quizAnswers.at( trueAnswerPosition ) = sentence.createQuestionTags( line );
            if( trueAnswerPosition == 0 ){

            }
        }
    }
    return quizzes;
}



std::vector <std::unordered_map <std::string, std::string>> tool::generateQuizzes()
{

}
*/


vector <string> tool::split (string stringToSplit, char spliter)
{
    vector <string> splittedStrings;
    auto startOfString = std::begin( stringToSplit );
    auto endOfString = std::end( stringToSplit );
    auto spaceCharacter = std::find (startOfString, endOfString, spliter);

    while (spaceCharacter != endOfString) {
        string currentString { startOfString, spaceCharacter};
        splittedStrings.push_back( currentString );
        while (*spaceCharacter == spliter ) {
            spaceCharacter++;
        }
        startOfString = spaceCharacter ;
        spaceCharacter = std::find (startOfString, endOfString, spliter);
    }
    splittedStrings.push_back( string (startOfString, endOfString) );
    return splittedStrings;
}



bool tool::isPastParticiple ( string word )
{
    return tool::checkVerbState( &pastParticiples, word, Dictionary::PAST_PARTICIPLE );
}



string tool::checkContractedForm( string verb, string nextWord, string nextNextWord )
{
    auto endOfWord       =  std::end    ( verb );
    auto startOfWord     =  std::begin  ( verb );
    auto apostrophe      =  std::find   ( startOfWord, endOfWord, '\'');
    auto contractedWord  =  std::string ( apostrophe + ONE_CHAR, endOfWord );
    auto apostropheFoud  = ( apostrophe  != endOfWord );

    if( apostropheFoud ){
        string wordBeforeApostrophe{ startOfWord, apostrophe };

        if( tool::isModal( wordBeforeApostrophe ) ) return wordBeforeApostrophe + "n't";
        
        auto unContract = [ contractedWord, nextWord ]
                          (string contractedVerb, string integralVerb, string negativeForm = VOID_STRING) -> string
        {
            if( contractedWord == contractedVerb ) {
                if( integralVerb == "am" && nextWord != "not" ) return "aren't";
                if( negativeForm.empty() ) negativeForm = integralVerb + "n't"s;
                return ( nextWord == "not"s ) ? integralVerb : negativeForm;
            }   
            else return string(VOID_STRING);
        };

        string nonContractedVerb(""s);
                                         nonContractedVerb  =  unContract  ( "ll"s, "will"s, "won't"s);//shall should be add to second verb for 'll contraction I must do it after
        if ( nonContractedVerb.empty())  nonContractedVerb  =  unContract  ( "re"s, "are"s );
        if ( nonContractedVerb.empty())  nonContractedVerb  =  unContract  ( "m"s , "am"s  );
        if ( nonContractedVerb.empty())  nonContractedVerb  =  unContract  ( "ve"s, "have"s);


        if( contractedWord == "d"s ){
            if( tool::isInList( &infinitiveVerbs, nextWord )) return "wouldn't"s;
            if( nextWord == "not"s && tool::isInList( &infinitiveVerbs, nextNextWord )) return "would"s;
            return ( nextWord == "not"s ) ? "had" : "hadn't" ;
        }

        if( contractedWord == "s"s ){

            auto isPastParticiple = []( string word ) -> bool {

                bool nextWordEndByEd = std::regex_match( word, std::regex(".*ed$"));
                if( nextWordEndByEd )
                        return tool::isRegularPreterit( string( word.begin(), word.end() - TWO_CHARS ));
                else    return tool::isPastParticiple( word );
            };

            if( isPastParticiple( nextWord ) ) return "hasn't"s;
            if( nextWord == "not"s && isPastParticiple( nextNextWord )) return "has"s;
            return ( nextWord == "not"s ) ? "is" : "isn't";
        }

        return nonContractedVerb;
    }
    return VOID_STRING;
}




void tool::removeEndDot( std::string &word ){
    auto dotPosition = std::find( word.begin(), word.end(), '.' );
    bool nextCharIsSpace = isspace( *(dotPosition + ONE_CHAR) ) ||
                          !isalnum( *(dotPosition + ONE_CHAR) );

    if( dotPosition != word.end() && nextCharIsSpace ){
        word = string{ word.begin(), dotPosition };
    }
}
