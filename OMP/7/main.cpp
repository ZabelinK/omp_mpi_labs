#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>


#include <omp.h>

class Matrix {
public:
    Matrix(const std::string& filename);

    int getAlgAdd(size_t i, size_t j) const;

    size_t dimetion;
private:
    int calcDet(const std::set<size_t>& rows, const std::set<size_t>& lines) const;

    std::vector<int> values;
};

Matrix::Matrix(const std::string& filename) :
    dimetion(0),
    values()
{
    std::ifstream input(filename);

    input >> dimetion;

    values.reserve(dimetion * dimetion);
    values.insert(values.begin(), std::istream_iterator<int>(input), std::istream_iterator<int>());
}

int Matrix::getAlgAdd(size_t i, size_t j) const
{
    if (dimetion == 1) {
        return 0;
    }

    if (dimetion == 2) {
        return values[0] * values[3] + values[1] * values[2];
    }

    std::set<size_t> rows;

    for (size_t i = 0; i < dimetion; ++i) {
        rows.insert(i);
    }

    std::set<size_t> lines(rows);

    rows.erase(j);
    lines.erase(i);

    return ((i + j) % 2 == 0 ? 1 : -1) * calcDet(rows, lines);
}

int Matrix::calcDet(const std::set<size_t>& rows, const std::set<size_t>& lines) const
{
    if (rows.size() != lines.size()) {
        throw std::logic_error("rows != lines");
    }

    if (rows.size() == 2) {
        return values[*lines.begin() * dimetion + *rows.begin()] * values[*(--lines.end()) * dimetion + *(--rows.end())]
            - values[*lines.begin() * dimetion + *(--rows.end())] * values[*(--lines.end()) * dimetion + *rows.begin()];
    }


    int det = 0;
    size_t i = 1;
    std::set<size_t> new_rows(rows);
    new_rows.erase(*rows.begin());

    for (auto line = lines.begin(); line != lines.end(); ++line) {
        std::set<size_t> new_lines(lines);
        new_lines.erase(*line);
        det += (i % 2 == 0 ? -1 : 1) * values[*line * dimetion + *rows.begin()] * calcDet(new_rows, new_lines); 
        ++i;
    }

    return det;
}

int main() 
{
    Matrix m("./text.txt");

    std::vector<int> values(m.dimetion * m.dimetion);

    #pragma omp parallel for
    for (int i = 0; i < values.size(); ++i) {
        values[i] = m.getAlgAdd(i / m.dimetion, i % m.dimetion);
    }

    for (int i = 0; i < values.size(); ++i) {
        if (i % m.dimetion == 0) {
            std::cout << '\n';
        }
        std::cout << values[i] << ' ';
    }

    std::cout << '\n';

    return 0;
}