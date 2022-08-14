#pragma once
#include "ControlBlock.h"
#include <fstream>

enum class isSaveBest
{
	save, not_save
};

class Web
{
public:
	explicit Web(const std::vector<size_t>& layers, std::vector<std::string> NeournsMeans, const std::string& filename = "");
	void Print() const;
	void Load(const std::string& filename);
	void Save(const std::string& filename);
	void ParseData(const std::string& filename, const std::pair<size_t, size_t>& index, std::string savefile, const isSaveBest& saver);
	void ControlTest(const std::string& filename, int NUM);
private:
	std::vector<ControlBlock*> Layers;
	void Solve();
	void SetStartColumn(std::fstream& filename, const size_t& Enum);
	void BackPropogation();
	void UpdateWeb();
	size_t StartColumn;
};

