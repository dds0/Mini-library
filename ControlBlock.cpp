#include "ControlBlock.h"

ControlBlock::ControlBlock(const size_t& EnumNeurons) : Neurons(EnumNeurons, 1)		{}

CommonControlBlock::CommonControlBlock(const size_t& i, const size_t& j) : MainWeights(i, j, "random"), ChangesWeights(i, j), SummError(j)
{
	Neurons.Resize(j, 1);
}

LastControlBlock::LastControlBlock(const size_t& i, std::vector<std::string> NeournsMeans)
{
	Neurons.Resize(i, 1);
	WhatNeouronsMeans = std::move(NeournsMeans);
}

void ControlBlock::Print() const
{
	size_t index = Neurons.GetSize().first;
	for (size_t i = 0; i < index; ++i)
		std::cout << Neurons[i][0] << "\n";
}

void CommonControlBlock::Print() const
{
	std::pair<size_t, size_t> index = MainWeights.GetSize();
	ControlBlock::Print();
	std::cout << "\n";
	for (size_t i = 0; i < index.first; ++i)
	{
		for (size_t j = 0; j < index.second; ++j)
			std::cout << MainWeights[i][j] << "\t";

		std::cout << "\n";
	}
	std::cout << "##################################################" << std::endl;
}

void LastControlBlock::Print() const
{
	ControlBlock::Print();
}

std::string ControlBlock::Save()
{
	std::stringstream ss;
	size_t Kol = this->Neurons.GetSize().first;

	for (size_t i = 0; i < Kol; i++)
		ss << Neurons[i][0] << "\t";
	ss << "\n";

	return ss.str();
}

std::string CommonControlBlock::Save()
{
	std::stringstream ss;
	ss << ControlBlock::Save();

	std::pair<size_t, size_t> index = MainWeights.GetSize();

	for (size_t i = 0; i < index.first; ++i)
	{
		for (size_t j = 0; j < index.second; ++j)
			ss << MainWeights[i][j] << "\t";

		ss << "\n";
	}
	return ss.str() + "\n";
}

std::string LastControlBlock::Save()
{
	return ControlBlock::Save() + "\n";
}

size_t ControlBlock::ReturnerBulk() const
{
	return this->Neurons.GetSize().first;
}

size_t CommonControlBlock::ReturnerBulk() const
{
	return ControlBlock::ReturnerBulk() + MainWeights.GetSize().first * MainWeights.GetSize().second;
}

size_t LastControlBlock::ReturnerBulk() const
{
	return ControlBlock::ReturnerBulk();
}

void ControlBlock::Load(std::stringstream& str)
{
	float temp;
	size_t index = Neurons.GetSize().first;

	for (size_t i = 0; i < index; i++)
	{
		str >> temp;
		Neurons[i][0] = temp;
	}
}

void CommonControlBlock::Load(std::stringstream& str)
{
	ControlBlock::Load(str);
	float temp;
	std::pair<size_t, size_t> index = MainWeights.GetSize();

	for (size_t i = 0; i < index.first; ++i)
	{
		for (size_t j = 0; j < index.second; ++j)
		{
			str >> temp;
			MainWeights[i][j] = temp;
		}
	}
}

void LastControlBlock::Load(std::stringstream& str)
{
	ControlBlock::Load(str);
}


Matrix CommonControlBlock::SendDataNextLayer()
{
	return this->Sigmoid(this->MainWeights * this->Neurons);
}

void LastControlBlock::SetRightAnswer(std::string& str)
{
	RightAnswer = std::move(str);
}

void ControlBlock::SetStartMatrix(Matrix m)
{
	Neurons = std::move(m);
}


Matrix ControlBlock::Sigmoid(Matrix m)
{
	std::pair<size_t, size_t> temp = m.GetSize();

	for (size_t i = 0; i < temp.first; ++i)
			m[i][0] = 1 / (1 + exp(-(displacement_neuron + m[i][0])));

	return m;
}


std::pair<float, std::vector<float>> LastControlBlock::MistakeFunc()
{
	float temp = 0;
	std::vector<float> v(this->Neurons.GetSize().first);
	std::pair<float, std::string> max_enum_and_what_it_mean = std::make_pair(Neurons[0][0], WhatNeouronsMeans[0]);
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (max_enum_and_what_it_mean.first < Neurons[i][0])
			max_enum_and_what_it_mean = std::make_pair(Neurons[i][0], WhatNeouronsMeans[i]);

		if (WhatNeouronsMeans[i] == RightAnswer)
		{
			v[i] = Neurons[i][0] - 1;
			temp += pow(v[i], 2);
		}
		else
		{
			v[i] = Neurons[i][0] - 0;
			temp += pow(v[i], 2);
		}
	}

	if (max_enum_and_what_it_mean.second == RightAnswer)
		++PercentOfRightAnswer;

	return std::make_pair(temp, v);
}

Matrix LastControlBlock::ComputeError() // матрица со значения столбцов  0: значение слоя, 1: ошибка, 2: дельта веса  = ошибка * sigmoid(x)dx (= sigmoid(x)*(1 - sigmoid(x)))
{
	auto temp = this->MistakeFunc();
	Matrix m(this->Neurons.GetSize().first, 1);
	++repeat_index;

	if (repeat_index == 1)
		main_web_error = temp.first;
	else
		main_web_error = ((repeat_index - 1) * main_web_error + temp.first) / repeat_index;

	for (size_t i = 0; i < m.GetSize().first; ++i)
		m[i][0] = temp.second[i] * Neurons[i][0] * (1 - Neurons[i][0]);

	return m;
}

void LastControlBlock::SetError(const Matrix& m) // should be empty
{

}

Matrix CommonControlBlock::ComputeError() //надо отдебажить
{
	Matrix m(this->Neurons.GetSize().first, 1);

	for (size_t i = 0; i < m.GetSize().first; ++i)
		m[i][0] = SummError[i] * Neurons[i][0] * (1 - Neurons[i][0]);

	return m;
}

void CommonControlBlock::SetError(const Matrix& m) //надо отдебажить
{
	float temp;
	std::pair<size_t, size_t> index = this->MainWeights.GetSize();
	for (size_t i = 0; i < index.first; ++i)
	{
		for (size_t j = 0; j < index.second; ++j)
		{
			temp = MainWeights[i][j] - Neurons[j][0] * m[i][0] * LEARNING_RATE;
			ChangesWeights[i][j] += temp - MainWeights[i][j];
			SummError[j] += temp * m[i][0];
		}
	}
}


void CommonControlBlock::UpdateLayer()
{
	auto index = MainWeights.GetSize();

	for (auto& s : SummError)
		s = 0;

	for (size_t i = 0; i < index.first; ++i)
	{
		for (size_t j = 0; j < index.second; ++j)
		{
			MainWeights[i][j] += ChangesWeights[i][j];
			ChangesWeights[i][j] = 0;
		}
	}
}


void LastControlBlock::UpdateLayer()
{
	++Generation;
	float temp = (static_cast<float>(PercentOfRightAnswer) / static_cast<float>(repeat_index)) * 100;
	std::cout << "Generation: " << Generation << "\t with MistakeFunction: " << main_web_error << "\t Percent of right answer: ";
	std::cout /*<< std::fixed << std::setprecision(2)*/<< temp <<"%"<< std::endl;
	repeat_index = 0;
	PercentOfRightAnswer = 0;
	main_web_error = 0;
}

void CommonControlBlock::FreeSummError()
{
	for (auto& s : SummError)
		s = 0;
}

size_t LastControlBlock::ReturnPercent() const
{
	return PercentOfRightAnswer;
}

void LastControlBlock::CheckTotalAnswer()
{
	float max = 0, it = 0;
	for (size_t i = 0; i < Neurons.GetSize().first; ++i)
	{
		if (max < Neurons[i][0])
		{
			max = Neurons[i][0];
			it = i;
		}
	}
	if (WhatNeouronsMeans[it] == RightAnswer)
		++PercentOfRightAnswer;
}