//This test case tests the checkFlush() function of the class Hand. 
//This function checks whether the given set of cards are Flush or not.
//The function returns true if it is true and false it it is not

//Compilation instructions: g++ test_example.cpp -o test
//                          ./test

#define BOOST_TEST_MODULE my_test_module
#include <boost/test/included/unit_test.hpp>
#include<algorithm>


struct Card
{
int suit;
int rank;
};

//This is the fucntion that is being tested
//Check for Flush
bool checkFlush(std::vector<Card> cards) {
    //Save suit of first card to compare to all other cardsd2But
    int suit = cards[0].suit;
    //Flag to be swapped if any of the cards fail either suit check or rank check.
    bool flag = true;
    for(int i = 0; i < 5; i++) {
        if(cards[i].suit != suit)
            flag = false;
    }
    return flag;
}



//test when condition are met to be the flush
BOOST_AUTO_TEST_CASE( my_test_case )
{
std::vector <Card>cards;
int i;
for (i=0;i<5;i++)
{
struct Card C;
C.suit=0;
C.rank=i+1;
cards.push_back(C);
}
    BOOST_CHECK( checkFlush(cards) == true );        // #1 continues on error


}


//test when conditions aren't met
BOOST_AUTO_TEST_CASE( second_test_case )
{
std::vector <Card>cards;
int i;
for (i=0;i<5;i++)
{
struct Card C;
C.suit=i;
C.rank=i+1;
cards.push_back(C);
}
    BOOST_CHECK( checkFlush(cards) == false );        // #1 continues on error


}

