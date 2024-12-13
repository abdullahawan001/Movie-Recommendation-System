#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Movie structure
struct Movie {
    string name;
    string genre;
    float rating;
    Movie* next;
    Movie* prev;
};

// Doubly linked list class for movies
class MovieList {
private:
    Movie* head;
    Movie* tail;

public:
    MovieList() : head(nullptr), tail(nullptr) {}

    // Add a new movie
    void addMovie(string name, string genre, float rating) {
        Movie* newMovie = new Movie{name, genre, rating, nullptr, nullptr};
        if (!head) {
            head = tail = newMovie;
        } else {
            tail->next = newMovie;
            newMovie->prev = tail;
            tail = newMovie;
        }
        saveToFile();
    }

    // Display all movies
    void displayMovies() {
        if (!head) {
            cout << "No movies to display.\n";
            return;
        }
        Movie* temp = head;
        cout << "\nMovies:\n----------------------------------\n";
        cout << "Name\t\tGenre\t\tRating\n";
        cout << "----------------------------------\n";
        while (temp) {
            cout << temp->name << "\t\t" << temp->genre << "\t\t" << temp->rating << "\n";
            temp = temp->next;
        }
    }

    // Search for a movie by name
    void searchMovie(string name) {
        Movie* temp = head;
        while (temp) {
            if (temp->name == name) {
                cout << "Movie found: " << temp->name << " (" << temp->genre << ") with rating " << temp->rating << "\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Movie not found.\n";
    }

    // Search movies by exact rating
    void searchMoviesByRating(float ratingThreshold) {
        if (!head) {
            cout << "No movies to search.\n";
            return;
        }

        Movie* temp = head;
        bool found = false;
        cout << "\nMovies with rating " << ratingThreshold << ":\n";
        cout << "----------------------------------\n";
        cout << "Name\t\tGenre\t\tRating\n";
        cout << "----------------------------------\n";

        while (temp) {
            if (temp->rating == ratingThreshold) {
                cout << temp->name << "\t\t" << temp->genre << "\t\t" << temp->rating << "\n";
                found = true;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No movies found with rating " << ratingThreshold << ".\n";
        }
    }

    // Delete a movie
    void deleteMovie(string name) {
        Movie* temp = head;
        while (temp && temp->name != name) {
            temp = temp->next;
        }

        if (!temp) {
            cout << "Movie not found.\n";
            return;
        }

        if (temp->prev) temp->prev->next = temp->next;
        if (temp->next) temp->next->prev = temp->prev;

        if (temp == head) head = temp->next;
        if (temp == tail) tail = temp->prev;

        delete temp;
        cout << "Movie deleted.\n";
        saveToFile();
    }

    // Edit a movie's details
    void editMovie(string oldName, string newName, string newGenre, float newRating) {
        Movie* temp = head;
        while (temp) {
            if (temp->name == oldName) {
                temp->name = newName;
                temp->genre = newGenre;
                temp->rating = newRating;
                cout << "Movie details updated successfully!\n";
                saveToFile();
                return;
            }
            temp = temp->next;
        }
        cout << "Movie not found.\n";
    }

    // Clear the entire movie list
    void clearMovies() {
        while (head) {
            Movie* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        cout << "All movies cleared.\n";
        saveToFile();
    }

    // Save movies to a file
    void saveToFile() {
        ofstream file("movies.txt");
        if (!file) {
            cout << "Error saving to file.\n";
            return;
        }
        Movie* temp = head;
        while (temp) {
            file << temp->name << "," << temp->genre << "," << temp->rating << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // Load movies from a file
    void loadFromFile() {
        ifstream file("movies.txt");
        if (!file) {
            cout << "No existing movie file found.\n";
            return;
        }
        string name, genre, line;
        float rating;
        while (getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.rfind(',');
            if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                name = line.substr(0, pos1);
                genre = line.substr(pos1 + 1, pos2 - pos1 - 1);
                rating = stof(line.substr(pos2 + 1));
                addMovie(name, genre, rating);
            }
        }
        file.close();
    }

    ~MovieList() {
        clearMovies();
    }
};

int main() {
    MovieList movieList;
    movieList.loadFromFile();

    int choice;
    string name, genre, oldName;
    float rating;

    do {
        cout << "\n1. Add Movie\n2. Search Movie by Name\n3. Search Movie by Rating\n4. Delete Movie\n5. Display Movies\n";
        cout << "6. Edit Movie\n7. Clear Movies\n8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter movie name: ";
                getline(cin, name);
                cout << "Enter genre: ";
                getline(cin, genre);
                cout << "Enter rating: ";
                cin >> rating;
                movieList.addMovie(name, genre, rating);
                break;
            case 2:
                cout << "Enter movie name to search: ";
                getline(cin, name);
                movieList.searchMovie(name);
                break;
            case 3:
                cout << "Enter the exact rating to search for: ";
                cin >> rating;
                movieList.searchMoviesByRating(rating);
                break;
            case 4:
                cout << "Enter movie name to delete: ";
                getline(cin, name);
                movieList.deleteMovie(name);
                break;
            case 5:
                movieList.displayMovies();
                break;
            case 6:
                cout << "Enter the current name of the movie to edit: ";
                getline(cin, oldName);
                cout << "Enter new name: ";
                getline(cin, name);
                cout << "Enter new genre: ";
                getline(cin, genre);
                cout << "Enter new rating: ";
                cin >> rating;
                movieList.editMovie(oldName, name, genre, rating);
                break;
            case 7:
                movieList.clearMovies();
                break;
            case 8:
                cout << "Exiting.\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    } while (choice != 8);

    return 0;
}