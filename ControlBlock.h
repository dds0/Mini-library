#pragma once
#include "Matrix.h"
#include <sstream>
#include <cmath>
#include <iomanip>
#define displacement_neuron 1
#define LEARNING_RATE 0.00001

class ControlBlock
{
public:
	ControlBlock(const size_t& EnumNeurons);
	ControlBlock() = default;
	virtual void Print() const;
	virtual std::string Save();
	virtual void Load(std::stringstream& str);
	virtual size_t ReturnerBulk() const;
	void SetStartMatrix(Matrix m);
	Matrix Sigmoid(Matrix m);

	virtual void SetError(const Matrix& m) = 0;
	virtual Matrix ComputeError() = 0;
	virtual void UpdateLayer() = 0;
protected:
	Matrix Neurons;
};

class CommonControlBlock : public ControlBlock
{
public:
	CommonControlBlock(const size_t& i, const size_t& j);
	void Print() const override;
	std::string Save() override;
	size_t ReturnerBulk() const override;
	void Load(std::stringstream& str) override;
	Matrix SendDataNextLayer();

	Matrix ComputeError() override;
	void SetError(const Matrix& m) override; 
	void FreeSummError();

	void UpdateLayer() override;
private:
	Matrix MainWeights;
	std::vector<float> SummError;
	Matrix ChangesWeights;
};


class LastControlBlock : public ControlBlock
{
public:
	void Print() const override;
	LastControlBlock(const size_t& i, std::vector<std::string> NeournsMeans);
	std::string Save() override;
	size_t ReturnerBulk() const override;
	void Load(std::stringstream& str) override;
	void SetRightAnswer(std::string& str);

	std::pair<float, std::vector<float>> MistakeFunc();
	Matrix ComputeError() override;
	void SetError(const Matrix& m) override; // should be empty

	void UpdateLayer() override;
	size_t ReturnPercent() const;
	void CheckTotalAnswer();
private:
	std::string RightAnswer;
	std::vector<std::string> WhatNeouronsMeans;
	float main_web_error = 0;
	size_t repeat_index = 0;
	size_t Generation = 0;
	size_t PercentOfRightAnswer = 0;
};
