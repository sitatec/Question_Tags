#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "include/error.hpp"

#define ONE_CHAR 1
#define TWO_CHARS 2
#define THREE_CHARS 3
#define FOUR_CHARS 4
#define VOID_STRING ""s
#define SPACE " "s
#define WHITESPACE " \n\r\t\f\v"s



 namespace tool
  {

    enum Dictionary { 
        PRETERIT,
        INFINITIVE,
        AUXILIARIES, 
        MODAL_VERBS,
        PAST_PARTICIPLE
    };

    void          removeEndDot(std::string& str);
    std::string   rtrim (const std::string& str);
    std::string   ltrim (const std::string& str);
    std::string   trim  (const std::string& str);
    std::string   toLowerCase (std::string stringToLower);
    std::string   toggleAffOrNeg (std::vector <std::string> dictionary, int positionInDictionary);
    std::string   toggleTense (std::string verb);
    std::string   presentToPreterit (std::string verb);
    std::string   preteritToPresent (std::string verb);

    std::vector   <std::string>*   getVerbsEndWithEs();
    std::vector   <std::string>*   getVerbsEndWithIes();
    std::vector   <std::string>*   getPreteritVerbs();
    std::vector   <std::string>*   getInfinitiveVerbs();
    std::vector   <std::string>*   getAuxiliariesList();
    std::vector   <std::string>*   getModalVerbsList();
    std::vector   <std::string>*   getersTemplate(std::vector <std::string>* list, Dictionary dictionary);

    std::vector   <std::string>   split(std::string string, char spliter = ' ' );
    
    bool isVerb                          (std::string  verb);
    bool isModal                         (std::string  verb , int* positionInDictionary = nullptr);
    bool isAuxiliary                     (std::string  verb , int* positionInDictionary = nullptr);
    bool isInThePresent                  (std::string  verb);
    bool isNegativeWord                  (std::string  word);
    bool isPastParticiple                (std::string  word);
    bool isPersonalPronoun               (std::string  word);
    bool isRegularPreterit               (std::string  verb);
    bool isIrregularPreterit             (std::string  verb);
    bool isThirdPersonSingular           (std::string  word);
    bool isIndefinitePluralPronoun       (std::string  word);
    bool isIndefiniteSingularPronoun     (std::string  word);
    bool hasDoubleConsonantInPreterit    (std::string  verb);
    bool conjugatedInThirdPersonSingular (std::string  verb);

    
    std::string toUnconjugated           (std::string  verb);
    std::string checkContractedForm      (std::string  verb , std::string nexWord, std::string nextNextWord);
    std::string conjugateInThirdPerson   (std::string  verb);


    std::unordered_map <Dictionary, std::string> getDictionariesPath();

    bool isVowel (char c);
    bool isPersonalPronoun (std::string str);
    void loadDictionaries();
    void loadDictionary(Dictionary dictionaryName);

    bool isInList( std::vector <std::string>* list , std::string const& entity,
                                                int* positionInList = nullptr );
    bool checkVerbState( std::vector <std::string>* list, std::string entity,
                         Dictionary dictionary, int* positionInList = nullptr);

//  std::unordered_map <std::string, std::vector <std::string>> generateAQuiz();
//  std::vector <std::unordered_map <std::string, std::string>> generateQuizzes();

}

//#include "../src/template/tools.tpp"


 /*

     bool isInList( std::unordered_map <std::string, Dictionary>* list , std::string const& entity,
                                                         int* positionInList = nullptr )
     {
         auto startOfList = std::begin( *list );
         auto endOfList = std::end( *list );
         auto itemPositionFound = std::find (startOfList, endOfList, entity );

         if( itemPositionFound != endOfList && positionInList != nullptr ){
             *positionInList = (itemPositionFound - startOfList) + 1 ;
             return true;
         }
         return false;
     }
 */


/*
    template <typename List, typename Entity>
    static bool isInList (List list , Entity entity) 
    {
        auto startOfList = std::begin( list );
        auto endOfList = std::end( list );
        auto itemPositionFound = std::find (startOfList, endOfList, entity );

        return ( itemPositionFound != endOfList ) ? true : false ; 
    }
  */  


/*   template <typename List, typename Entity, typename Dictionary>
    bool checkVerbType(List list, Entity entity, Dictionary dictionary, Type type,
                        Type* verbType, int* positionInList = nullptr ) 
    {    
        return checkVerbState( list, entity, dictionary, type, verbType, positionInList );
    } 


    template <typename List, typename Entity, typename Dictionary>
    bool checkVerbTense(List list, Entity entity, Dictionary dictionary, Tense tense, Tense* verbTense) 
    {    
        return checkVerbState( list, entity, dictionary, tense, verbTense );
    }
*/
