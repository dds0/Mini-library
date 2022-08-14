#include "Web.h"
#include <iostream>

int main()
{
	srand(time(NULL));
	Web web({ 784,24,48,24,10 }, { "0","1","2","3","4","5","6","7","8","9" }, "BestGeneration");//BestGeneration    Save_5layers_from_BigDataSet_nostartZERO
	//web.Load("test_save");
	//for (size_t i =0; i < 2; ++i)
		//web.ParseData("BigDataSet", { 120,500 }, "Save_5layers_from_BigDataSet_nostartZERO", isSaveBest::not_save);
	web.ControlTest("DataSet", 10'000);
	return 0;
}

// 1 проход по выборке 60.000 +- 15 минут
