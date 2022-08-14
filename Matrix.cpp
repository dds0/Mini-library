#include "Matrix.h"

Matrix::Matrix(const size_t& i, const size_t& j, const std::string& type) : W(i,std::vector<float>(j))
{
	isInit = true;

	if (type == "random")
		setW(i, j, MatrixFlag::Rand);
	else
		setW(i, j, MatrixFlag::Empty);
}

Matrix::Matrix() : W(0, std::vector<float>(0)), isInit(true)	{}

std::vector<float>& Matrix::operator[](const size_t& index)
{
	return W[index];
}

const std::vector<float>& Matrix::operator[](const size_t& index) const
{
	return W[index];
}

void Matrix::setW(const size_t& I, const size_t& J, const MatrixFlag& flag)
{
	if (flag == MatrixFlag::Rand)
	{
		// -1 to +1
		for (size_t i = 0; i < I; ++i)
		{
			for (size_t j = 0; j < J; ++j)
			{
				W[i][j] = (rand() % 200 - 100) / 100.0;
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	try
	{
		if (!m.isInit)
			throw std::exception("incorrect initialization");

		for (const auto& i : m.W)
		{
			for (const auto& j : i)
				os << j << "\t";
			os << "\n";
		}

		return os;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return os;
	}
}

std::pair<size_t, size_t> Matrix::GetSize() const
{
	return std::make_pair(this->W.size(), this->W[0].size());
}

Matrix operator*(const Matrix& left, const Matrix& right)
{
	try
	{
		if (left.W[0].size() != right.W.size())
			throw std::exception("can't multiply");

		size_t I = left.W.size(), J = right.W[0].size(), Jleft = left.W[0].size();

		Matrix temp(I, J);

		for (size_t i = 0; i < I; ++i)
		{
			for (size_t j = 0; j < J; ++j)
			{
				for (size_t k = 0; k < Jleft; ++k)
					temp[i][j] += left[i][k] * right[k][j];
			}
		}

		return temp;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return Matrix(0, 0);
	}

}

void Matrix::Resize(const size_t& i, const size_t& j, const std::string& type)
{
	W.resize(i);
	for (auto& s : W)
		s.resize(j);

	if (type == "random")
		this->setW(i, j, MatrixFlag::Rand);
}
