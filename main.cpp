#include <iostream>
#include <cstdlib> // needed to use srand and rand
#include <ctime> // needed to use time
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// In production I would make fetch requests to the server for each book/video that is searched... since that's not within the scope, I picked the top 9 most popular books/videos according to a Google search. Also, the values have been pre-sorted alphabetically.
string bookDatabase[9] = 
{"A Tale of Two Cities", "Alice's Adventures in Wonderland", "Don Quixote", 
"Harry Potter and the Philosopher's Stone", "Harry Potter and the Deathly Hallows", "Moby Dick", "The Bible", "The Da Vinci Code", "The Lord of the Rings"};
string videoDatabase[9] = 
{"Avatar", "Ben-Hur", "E. T. The Extra-Terrestrial",
"Gone With the Wind", "Jaws", "Star Wars: Episode IV - A New Hope", 
"The Sound of Music", "The Ten Commandments", "Titanic"};

// Customer class:
class Customer
{
  private:
      // in production a GET request would have been made to the server and these attributes would be initialized to match what's stored in the database
      bool bookMember;
      bool videoMember;
      int customerId;

  public:
      Customer () {
        // I would never do this in production, but since there's no server/database 
        unsigned seed;
        seed = time(0);
        srand(seed);

        this -> customerId = (rand() % 9999999) + 1;
        this -> bookMember = false;
        this -> videoMember = false;
      }

      int getId () {
        return this -> customerId;
      }

      bool getBookMember () {
        return this -> bookMember;
      }

      bool getVideoMember () {
        return this -> videoMember;
      }

      void setBookMember () {
        this -> bookMember = !(this -> bookMember);
      }

      void setVideoMember () {
        this -> videoMember = !(this -> videoMember);
      }
};

class Product
{
    string type;
    string title;
    int price;

public:
    Product (string type, string title, int price) {
      // wouldn't do this in production
      unsigned seed;
        seed = time(0);
        srand(seed);

      this -> type = type;
      this -> price = price;
      // book/video Memberships will pass in empty strings in place of the title parameter
      if (title != "") {
        this -> title = title;
      }
    }

    // * Get Methods
    string getType () {
      return this -> type;
    }

    string getTitle () {
      return this -> title;
    }

    int getPrice () {
      return this -> price;
    }

    // returns a vector containing two strings (str1, str2)
    string getDescription () {
      return (this -> type + " " + this -> title);
    }
};

class Purchase 
{
    int purchaseId;
    int total_price;
    vector<string> line_items;

public:
    Purchase (int total_price, vector<string> line_items) {
        unsigned seed;
        seed = time(0);
        srand(seed);

        this -> purchaseId = (rand() % 9999999) + 1;
        this -> total_price = total_price;

        this -> line_items = line_items;
    }

    // Get Methods
    int getId () {
        return this -> purchaseId;
    }

    int getPrice () {
        return this -> total_price;
    }
 
    vector<string> getLineItems () {
        return this -> line_items;
    }
};

// Function Prototypes
void chosePath (int &option);

int main() {
  
  // Initiate customer object
  Customer cust;

  // set purchasing boolean (controls menu)
  bool purchasing = true;
  int total_price = 0;
  int book_price = 5, video_price = 5, book_club_price = 5, video_club_price = 5;
  vector<string> cart_description;

  while (purchasing == true) 
  {
    cout << "\n1) Book \n2) Video\n3) Book Club Membership\n4) Video Club Membership\n5) Checkout\n" << endl;
    int option;
    chosePath(option);
			
    // For purchasing option (1-4):
    //  Create an object of the product
    //  Add the product info to the cart_description
    //  increment the total price

    if (option == 1 ) { 
      string bookEntry;
      cout << "Enter a book to purchase: ";
      getline (cin, bookEntry);
      // O(log(n))
      // The binary search is case sensitive, in production I would write my own that casts all the words to lowercase
      if (binary_search(bookDatabase, bookDatabase+9, bookEntry)) {
        Product prod("Book", "\"" + bookEntry + "\"", book_price);
        cart_description.push_back(prod.getDescription());
        total_price += prod.getPrice();
      }
      else {
        cout << "We do not carry this book." << endl;
      }
      
    }
    else if (option == 2) {
      string videoEntry;
      cout << "Enter a video to purchase: ";
      getline (cin, videoEntry);
      if (binary_search(videoDatabase, videoDatabase+9, videoEntry)) {
        Product prod("Video", "\"" + videoEntry + "\"", video_price);
        cart_description.push_back(prod.getDescription());
        total_price += prod.getPrice();
      }
      else {
        cout << "We do not carry this video." << endl;
      }
      
    }
    else if (option == 3) { 
      if (cust.getBookMember() == false) {
        cust.setBookMember();
        Product prod("Book Club Membership", "", book_club_price);
        cart_description.push_back(prod.getDescription());
        total_price += prod.getPrice();
      }
      else {
        cout << "You are already a Book Club Member." << endl;
      }
      
    }
    else if (option == 4) {
      if (cust.getVideoMember() == false) {
        cust.setVideoMember();
        Product prod("Video Club Membership", "", video_club_price);
        cart_description.push_back(prod.getDescription());
        total_price += prod.getPrice();
      }
      else {
        cout << "You are already a Video Club Member." << endl;
      }
      
    }
    else if (option == 5) {
      purchasing = false;
    }
    else {
      cout << "\nInvalid Entry, please chose again." << endl;
    }
  }

  if (total_price == 0 && cart_description.size() == 0) {
    cout << "\nNo purchase made, have a great day!" << endl;
    return 0;
  }
  Purchase cart(total_price, cart_description);

  // Output
  cout << "\n\nReceipt:\n--------------------------\n\n";

  cout << "ID: #" << cart.getId() << endl;
  cout << "Total Price: $" << cart.getPrice() << endl;
  cout << "Customer ID: #" << cust.getId() << endl << endl;

  cout << "Items purchased:" << endl << endl;
  // O(n)
  for (string line_item : cart.getLineItems()) {
    cout << line_item << endl;
  }

  return 0;
}

void chosePath (int &option) {
  cout << "Enter a menu option: ";
  cin >> option;
  while (cin.fail())	{
		cin.clear();
		cin.ignore(256,'\n');
		cout << "Please re-select an option: ";
		cin >> option;
    cout << endl;
  }
  cin.clear();
  cin.ignore(256,'\n');
}



