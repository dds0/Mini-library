#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <map>

enum class MatrixFlag
{
	Rand, Empty
};


class Matrix
{
public:
	explicit Matrix(const size_t& i, const size_t& j, const std::string& type = "");
	Matrix();
	std::pair<size_t, size_t> GetSize() const;
	void Resize(const size_t& i, const size_t& j, const std::string& type = "");

	std::vector<float>& operator[](const size_t& index);
	const std::vector<float>& operator[](const size_t& index) const;

	friend Matrix operator*(const Matrix& left, const Matrix& right);
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

private:
	std::vector<std::vector<float>> W;
	void setW(const size_t& I, const size_t& J, const MatrixFlag& flag);

	bool isInit = false;
};
