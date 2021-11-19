#include <iostream>
#include <optional>
#include <tuple>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <sstream>

using namespace std;

//Task 1 ------------------------------------------------------------------------------------------

struct Person //
{

    std::string m_f_name;
    std::string m_l_name;
    std::optional<std::string> patronimic;

    
    
    std::string GetFirstName() const { return m_f_name; }
    std::string GetLastName() const { return m_l_name; }
    std::optional<std::string> GetPatronimic() const { return patronimic; }

    Person ReadFromStream(std::istringstream& iss)
    {
        Person person;
        std::string temp;
        if (iss.peek() == ' ')
            iss.ignore(1);
        iss >> person.m_l_name >> person.m_f_name >> temp;
        if (temp != "-")
            person.patronimic.emplace(temp);
        return person;
    }

    void SetPerson(std::istringstream& iss)
    {
        std::string temp;
        iss >> m_l_name >> m_f_name >> temp;
        if (temp == "-")
            return;
        else
            patronimic.emplace(temp);
    }

    friend std::ostream& operator<< (std::ostream& out, const Person& person);

    friend bool operator< (const Person& p1, const Person& p2);

    friend bool operator== (const Person& p1, const Person& p2);

    
};
//Person::Person() :m_l_name(""), m_f_name(""), patronimic(std::nullopt) {}
//Person::Person(std::string l_name, std::string f_name, std::string patr) : m_l_name{ std::move(l_name) }, m_f_name{ std::move(f_name) }, patronimic{ std::move(patr) } {}

bool operator< (const Person& p1, const Person& p2) {
    return std::tie(p1.m_l_name, p1.m_f_name, p1.patronimic) <
        std::tie(p2.m_l_name, p2.m_f_name, p2.patronimic);
}

bool operator== (const Person& p1, const Person& p2) {
    return std::tie(p1.m_l_name, p1.m_f_name, p1.patronimic) ==
        std::tie(p2.m_l_name, p2.m_f_name, p2.patronimic);
}



//Task 2 ---------------------------------------------------------------------------------------------

class PhoneNumber
{
private:
    int m_country;
    int m_city;
    std::string m_number;
    std::optional<int> m_number2;
public:
    //конструктор
    PhoneNumber() : m_country(0), m_city(0), m_number("0"), m_number2(std::nullopt) {}
    PhoneNumber(int country, int city, std::string number, std::optional<int> number2) : m_country{ country }, m_city{ city }, m_number{ std::move(number) }, m_number2{ number2 }{}

    int GetCountry() const
    {
        return m_country;
    }
    int GetCity() const
    {
        return m_city;
    }
    std::string GetNumber() const
    {
        return m_number;
    }
    std::optional<int> GetNumber2() const
    {
            if (const int add_num = m_number2.value())
                return add_num;
            else
                return m_number2;
        
    }
    void SetNumber(std::istringstream& iss)
    {
        std::string temp;
        iss >> m_country >> m_city >> m_number >> temp;

        if (temp == "-")
            return;
        else
            m_number2.emplace(std::stoi(temp));
    }
    PhoneNumber ReadFromStream(std::istringstream& iss)
    {
        PhoneNumber number0;
        if (iss.peek() == ' ')
            iss.ignore(1);
        iss >> number0.m_country >> number0.m_city >>
            number0.m_number;
        if (iss.peek() != '-') {
            iss >> number0.m_number2.emplace();
        }
        return number0;
    }

    friend std::ostream& operator<< (std::ostream& out, const PhoneNumber& phonenumber);
    friend bool operator< (const PhoneNumber& num1, const PhoneNumber& num2);
   
};
bool operator< (const PhoneNumber& num1, const PhoneNumber& num2)
{
    return std::tie(num1.m_country, num1.m_city, num1.m_number, num1.m_number2) <
        std::tie(num2.m_country, num2.m_city, num2.m_number, num2.m_number2);
}

//PhoneNumber::PhoneNumber() : m_country(0), m_city(0), m_number("0"), m_number2(std::nullopt) {} 
// 
//со 2 заданием я конкретно запутался, я не вижу что он от меня хочет, он пишет что отсутствуют элементы конструктора для инт, хотя они у  меня есть
//ошибок программа не выдает, но в мейне параметры задать не дает тоже, я до талого убирал и добавлял все функции подряд и у меня замылились глаза

std::ostream& operator<< (std::ostream& out, const PhoneNumber& phonenumber)
{
    out << '+' << phonenumber.m_country << " (" << phonenumber.m_city << ") " << phonenumber.m_number << ' ';
    if (phonenumber.m_number2.has_value())
    {
        out << phonenumber.m_number2.value();
    }
    return out;
}

//task 3 ---------------------------------------------------------------------------------------------------------------

class PhoneBook
{
private:
    std::string string;
    std::vector<std::pair<Person, PhoneNumber>> phonebook;
public:

    void File(std::ifstream& file)
    {

        while (!file.eof())
        {
            std::getline(file, string);
            std::istringstream istringstr(string);
            Person person;
            PhoneNumber number;
            person.SetPerson(istringstr);
            number.SetNumber(istringstr);
            phonebook.push_back(std::make_pair(person, number));
        }
    }
    PhoneBook(std::ifstream& file)
    {
        File(file);
        if (!file)
            std::cerr << "Cannot open the file\n";
    }


    void SortByName()
    {
        std::sort(phonebook.begin(), phonebook.end(), [](std::pair<Person, PhoneNumber> person1,
            std::pair<Person, PhoneNumber> person2)
            {
                return person1.first < person2.first;
            }
        );
    }

        void SortByPhone()
        {
            std::sort(phonebook.begin(), phonebook.end(), [](std::pair<Person, PhoneNumber> number1,
                std::pair<Person, PhoneNumber> number2)
                {
                    return number1.second < number2.second;
                }
            );
        }

        std::tuple<std::string,PhoneNumber> GetPhoneNumber(const std::string& surname)
        {
            size_t count = 0;
            const PhoneNumber* number{};
            auto lambda = [&](auto& entry)
            {
                if (entry.first.GetLastName() == surname)
                {
                    count++;
                    number = &entry.second;
                }
            };
            std::for_each(phonebook.begin(), phonebook.end(), lambda);
            if (count == 0)
                return { "None found!", {} };
            else if (count > 1)
                return { "Found more than 1!", *number };
            return { "", *number };
        }
        void ChangePhoneNumber(const Person& person, PhoneNumber new_number)
        {
            auto user = std::find_if(phonebook.begin(), phonebook.end(), [&person](std::pair<Person, PhoneNumber>& user)
                {
                    return user.first == person;
                }
            );

            if (user != phonebook.end())
                user->second = new_number;
        }

    friend std::ostream& operator<< (std::ostream& out, PhoneBook& book);
};

std::ostream& operator<< (std::ostream& out, PhoneBook& book)
{
    for (const auto& iter : book.phonebook) {
        out << iter.first << ' ' << iter.second << std::endl;
    }
    return out;
}

int main()
{
   std::ifstream file("ХХХ"); // путь к файлу PhoneBook.txt
    PhoneBook book(file);
    std::cout << book;

    std::cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    std::cout << book;

    std::cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    std::cout << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname) {
        std::cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            std::cout << std::get<1>(answer);
        else
            std::cout << std::get<0>(answer);
        std::cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Nerabochaja");
    print_phone_number("Pomojka");

    std::cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    std::cout << book;

}
