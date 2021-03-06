#include <vector>
#include <stdexcept>

#include "reversi.h"

using namespace std;


Square& Square::operator=(SquareValue value)
{
	value_ = value;
	return *this;
}

bool Square::operator==(SquareValue value) const
{
	return (value_ == value);
}

bool Square::operator!=(SquareValue value) const
{
	return (value_ != value);
}

void Square::flip()
{
    switch (value_)
    {
    case WHITE:
        value_ = BLACK;
        break;
    case BLACK:
        value_ = WHITE;
        break;
    default:
        break;
    }
}

Square::SquareValue opposite_color(Square::SquareValue value)
{
    switch (value)
    {
    case Square::WHITE:
        return Square::BLACK;
    case Square::BLACK:
        return Square::WHITE;
    default:
        throw invalid_argument("Illegal square value");
    }
}

std::ostream& operator<<(std::ostream& out, const Square& square)
{
	if (square.value_ == FREE)
		out << '_';
	else if (square.value_ == BLACK)
		out << 'B';
	else(square.value_ == WHITE)
		out << 'W';
	return out;
}

Board::Board(size_t s)
{
	dimension_ = s;
	squares_ = new Square * [dimension_];
	for (int i = 0; i < dimension_; i++)
	{
		squares_[i] = new Square[dimension_];
	}
	for (int j = 0; j < dimension_; j++)
		for (int k = 0; k < dimension_; k++)
			squares_[j][k] = Square::FREE;
	squares_[dimension_ / 2][dimension_ / 2] = Square::BLACK;
	squares_[dimension_ / 2 - 1][dimension_ / 2] = Square::WHITE;
	squares_[dimension_ / 2][dimension_ / 2 - 1] = Square::WHITE;
	squares_[dimension_ / 2 - 1][dimension_ / 2 - 1] = Square::BLACK;
}

Board::Board(const Board& rhs)
{
	/*allocate 2d array*/
	dimension_ = rhs.dimension_;
	squares_ = new Square * [dimension_];
	for (int i = 0; i < dimension_; i++)
	{
		squares_[i] = new Square[dimension_];
	}

	for (int i = 0; i < dimension_; i++)
		for (int j = 0; j < dimension_; j++)
			squares_[i][j] = rhs.squares_[i][j];
}

Board& Board::operator=(const Board& rhs)
{
	if (this == &rhs)
		return *this;
	if (squares_ != NULL)
	{
		for (int i = 0; i < dimension_; i++)
			delete[] squares_[i];
		delete[] squares_;
	}
	dimension_ = rhs.dimension_;
	squares_ = new Square * [dimension_];
	for (int i = 0; i < dimension_; i++)
	{
		squares_[i] = new Square[dimension_];
	}

	for (int i = 0; i < dimension_; i++)
		for (int j = 0; j < dimension_; j++)
			squares_[i][j] = rhs.squares_[i][j];
	return *this;
}

Board::~Board()
{
	for (int i = 0; i < dimension_; i++)
		delete[] squares_[i];
	delete[] squares_;
}

Square& Board::operator()(char row, size_t column)
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

Square const& Board::operator()(char row, size_t column) const
{
    if (!is_valid_location(row, column))
    {
        throw out_of_range("Bad row index");
    }
    size_t row_index = row_to_index(row);
    return squares_[row_index][column - 1];
}

bool Board::is_legal_and_opposite_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] != Square::FREE && squares_[row_index][column - 1] != turn;
    }
    return false;
}

bool Board::is_legal_and_same_color(
    char row, size_t column, Square::SquareValue turn) const
{
    if (is_valid_location(row, column))
    {
        size_t row_index = row_to_index(row);
        return squares_[row_index][column - 1] == turn;
    }
    return false;
}

bool Board::is_valid_location(char row, size_t column) const
{
    size_t row_index = row_to_index(row);
    return row_index < dimension_ && column - 1 < dimension_;
}

Checkpoint::Checkpoint(const Board& b, Square::SquareValue turn) :
    board_(b),
    turn_(turn)
{}

ostream& Board::print(ostream& out) const
{
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        if (i < 10)
        {
            out << " ";
        }
        else
        {
            out << (i / 10);
        }
    }
    out << endl;
    out << "  ";
    for (size_t i = 1; i <= dimension_; i++)
    {
        out << (i % 10);
    }
    out << endl;
    for (size_t i = 0; i < dimension_; i++)
    {
        out << (char)('a' + i) << ':';
        for (size_t k = 0; k < dimension_; k++)
        {
            out << squares_[i][k];
        }
        out << endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Board& board)
{
	return board.print(out);
}

Reversi::Reversi(size_t size)
{
	Board temp(size);
	turn_ = Square::BLACK;
}

void Reversi::play()
{
	bool a == true;
	while (a)
	{
		prompt();
		char tempc;
		cin >> tempc;
		if (tempc == 'p')
		{
			char row;
			int column;
			cin >> row >> column;
			if (is_legal_choice(row, column, turn_))
			{
				const size_t direction_count = 8;
				const int direction_row[] = { -1, -1,  0, +1, +1, +1,  0, -1 };
				const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1 };
				// Now check in each directions
				for (size_t d = 0; d < direction_count; d++)
				{
					vector<char> charV;
					vector<int> intV;
					// Where we're checking
					char cursor_row = row + direction_row[d];
					size_t cursor_column = column + direction_column[d];

					// Move towards the direction while we're on the opposite color
					bool found_opposite = false;
					while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn_))
					{
						found_opposite = true;
						charV.push_back(cursor_row);
						intV.push_back(cursor_column);
						cursor_row += direction_row[d];
						cursor_column += direction_column[d];
					}

					// Check if the next thing after is our color
					bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn_);

					// If this direction is valid, the move is valid, so short circuit and return
					if (found_opposite && found_same)
					{
						for (int i = 0; i < charV.size(); i++)
						{
							board_(charV[i], intV[i]).flip();
						}
					}
				}
				if (turn_ == Square::BLACK)
					turn_ = Square::WHITE;
				else
					turn_ = Square::BLACK;
			}
		}
		else if (tempc == 'c')
		{
			save_checkpoint();
		}
		else if (tempc == 'u')
		{
			undo();
		}
		else
		{
			a = false;
		}
		if (is_game_over())
			a = false;
	}
	win_loss_tie_message(countWhite(), countBlack());
}

void Reversi::prompt() const
{
    cout << board_ << endl;
    cout << (turn_ == Square::BLACK ? "B" : "W");
    cout << " - Enter 'p r/c', 'q', 'c', 'u':" << endl;
}

void Reversi::win_loss_tie_message(size_t white_count, size_t black_count)
{
    cout << board_ << endl;
    if (white_count > black_count)
    {
        cout << "W wins" << endl;
    }
    else if (white_count < black_count)
    {
        cout << "B wins" << endl;
    }
    else
    {
        cout << "Tie" << endl;
    }
    cout << "W=" << white_count << "/B=" << black_count << endl;
}

bool Reversi::is_legal_choice(char row, size_t column, Square::SquareValue turn) const
{
    // Vectors for each cardinal direction
    const size_t direction_count = 8;
    const int direction_row[] =    {-1, -1,  0, +1, +1, +1,  0, -1};
    const int direction_column[] = { 0, -1, -1, -1,  0, +1, +1, +1};

    // Make sure location is free
    if (board_(row, column) != Square::FREE)
    {
        return false;
    }

    // Now check in each directions
    for (size_t d = 0; d < direction_count; d++)
    {
        // Where we're checking
        char cursor_row = row + direction_row[d];
        size_t cursor_column = column + direction_column[d];

        // Move towards the direction while we're on the opposite color
        bool found_opposite = false;
        while (board_.is_legal_and_opposite_color(cursor_row, cursor_column, turn_))
        {
            found_opposite = true;
            cursor_row += direction_row[d];
            cursor_column += direction_column[d];
        }

        // Check if the next thing after is our color
        bool found_same = board_.is_legal_and_same_color(cursor_row, cursor_column, turn_);

        // If this direction is valid, the move is valid, so short circuit and return
        if (found_opposite && found_same) {
            return true;
        }
    }
    return false;
}

void Reversi::save_checkpoint()
{
	Checkpoint temp(board_, turn_);
	history_.push_back(temp);
}

void Reversi::undo()
{
	if (history_.size() == 0)
		return;
	else
	{
		board_ = history_.back().board_;
		turn_ = history_.back().turn_;
		history_.pop_back();
	}
}

size_t Reversi::countWhite()
{
	int count = 0;
	for (char i = 'a'; i < ('a' + board_.dimension(); i++)
	{
		for (size_t j = 0; j < board_.dimension(); j++)
		{
			if (board_(i, j) == Square::WHITE)
				count++;
		}
	}
}

size_t Reversi::countBlack()
{
	int count = 0;
	for (char i = 'a'; i < ('a' + board_.dimension()); i++)
	{
		for (size_t j = 0; j < board_.dimension(); j++)
		{
			if (board_(i, j) == Square::BLACK)
				count++;
		}
	}
}

bool Reversi::is_game_over() const
{
    for (unsigned char row = 'a'; row < ('a' + board_.dimension()); row++)
    {
        for (size_t column = 1; column <= board_.dimension(); column++)
        {
            if (is_legal_choice(row, column, turn_))
            {
                return false;
            }
        }
    }
    return true;
}



