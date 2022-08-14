#include "Web.h"

Web::Web(const std::vector<size_t>& layers, std::vector<std::string> NeournsMeans, const std::string& filename)
{
	try
	{
		if (layers[layers.size() - 1] != NeournsMeans.size())
			throw std::exception("different enum of neourons.");

		StartColumn = layers[0];

		for (size_t i = 0; i < layers.size() - 1; ++i)
			this->Layers.push_back(new CommonControlBlock(layers[i + 1], layers[i]));

		this->Layers.push_back(new LastControlBlock(layers[layers.size() - 1], NeournsMeans));


		if (!filename.empty())
			this->Load(filename);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Web::Print() const
{
	for (const auto& s : Layers)
		s->Print();
}

void Web::Save(const std::string& filename)
{
	try
	{
		std::fstream file;
		file.open(filename + ".txt", std::ios::out);
		std::string temp;
		if (file.is_open())
		{
			for (const auto& s : Layers)
				file << s->Save();
		}
		else
			throw std::exception("can't open file.");

		file.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Web::Load(const std::string& filename)
{
	try
	{
		std::fstream file;
		file.open(filename + ".txt", std::ios::in);
		std::string temp;
		if (file.is_open())
		{
			for (const auto& s : Layers)
			{
				std::stringstream ss;
				size_t zifr = s->ReturnerBulk();
				float temp;

				while (zifr > 0)
				{
					file >> temp;
					ss << temp << "\t";
					--zifr;
				}
				s->Load(ss);
			}
		}
		else
			throw std::exception("can't open file.");

		file.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Web::Solve()
{
	for (size_t i = 1; i < Layers.size(); ++i)
		Layers[i]->SetStartMatrix(dynamic_cast<CommonControlBlock*>(Layers[i - 1])->SendDataNextLayer());
}

void Web::ParseData(const std::string& filename, const std::pair<size_t,size_t>& index, std::string savefile, const isSaveBest& saver)
{
	try
	{
		std::fstream file;
		file.open(filename + ".txt", std::ios::in);
		size_t enumLayersIndex = Layers.size() - 1, maxPercent = 0, iterator = 1;
		float average = 0;
		auto last = dynamic_cast<LastControlBlock*>(Layers[enumLayersIndex]);
		if (file.is_open())
		{
			for (size_t i = 0; i < index.first; ++i)
			{
				for (size_t j = 0; j < index.second; ++j)
				{
					this->SetStartColumn(file, StartColumn);
					this->Solve();
					this->BackPropogation();

				}
				if (last->ReturnPercent() > maxPercent)
				{
					maxPercent = last->ReturnPercent();
					if (saver == isSaveBest::save)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					{
						this->Save("BestGeneration");
						std::cout << "Best current generation save("<< iterator <<"). Max percent of true value: " << static_cast<float>(maxPercent) / (index.second / 100) << "%." << std::endl;
					}
				}

				if (iterator != 1)
					average = ((iterator - 1) * average + static_cast<float>(last->ReturnPercent())) / iterator;
				else
					average = last->ReturnPercent();

				this->UpdateWeb();

				if (isSaveBest::not_save == saver && iterator == index.first / 10)
				{
					this->Save(savefile);
					std::cout << "Max percent of true value last " << iterator << " generation: " << static_cast<float>(maxPercent) / (index.second / 100);
					std::cout<< "%" << "\tAverage ~ " << static_cast<size_t>(average/(index.second / 100)) << "%" << "\tData save." << std::endl;
					iterator = 1;
				}
				else
					++iterator;
			}
		}
		else
			throw std::exception("can't open file.");

		file.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Web::SetStartColumn(std::fstream& filename, const size_t& Enum)
{
	float temp;
	std::string Answer;
	filename >> Answer;
	dynamic_cast<LastControlBlock*>(this->Layers[Layers.size() - 1])->SetRightAnswer(Answer);
	Matrix cont(StartColumn, 1);
	for (size_t i = 0; i < StartColumn; ++i)
	{
		filename >> temp;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		/*if (temp > 0)*/
			temp = 1 / (1 + exp(-temp));
		cont[i][0] = temp;
	}
	this->Layers[0]->SetStartMatrix(cont);
}


void Web::BackPropogation()
{
	for (size_t i = Layers.size() - 1; i > 0; --i)
		Layers[i - 1]->SetError(Layers[i]->ComputeError());

	for (size_t i = 0; i < Layers.size() - 1; ++i)
		dynamic_cast<CommonControlBlock*>(Layers[i])->FreeSummError();

}

void Web::UpdateWeb()
{
	for (const auto& s: Layers)
		s->UpdateLayer();
}

void Web::ControlTest(const std::string& filename, int NUM)
{
	try
	{
		std::fstream file;
		file.open(filename + ".txt", std::ios::in);
		if (file.is_open())
		{
			for (size_t i = 0; i < NUM; ++i)
			{
				this->SetStartColumn(file, StartColumn);
				this->Solve();
				dynamic_cast<LastControlBlock*>(Layers[Layers.size() - 1])->CheckTotalAnswer();
			}
			size_t tt = dynamic_cast<LastControlBlock*>(Layers[Layers.size() - 1])->ReturnPercent();
			std::cout <<"Percent of true value: "<< static_cast<float>(tt) / static_cast<float>(NUM) * 100 <<"%" << std::endl;
		}
		else
			throw std::exception("can't open file.");

		file.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}