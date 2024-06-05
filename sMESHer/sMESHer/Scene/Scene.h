#pragma once
#include "../Mesh/Model.h"
#include <vector>
#include "../Light/Light.h"





class Scene {
public:
	static void QsortRecursive(Model** array, int size);
	static int InterpolateSearch(int searchLength) noexcept;
	static void XORShifrate(char* source, int size) noexcept;

	static void Render() noexcept;

	static void Clear() noexcept;
	static void RecreateNames() noexcept;
	static void SaveAll() noexcept;

	static void SortModels() noexcept;
	static void BinarySearchModels(char* searchName, int length) noexcept;

	static void UpdateLight() noexcept;
	static void InitLight() noexcept;
	static void TerminateLight() noexcept;

	static void ShifrateFile() noexcept;

	static std::vector<Model*> m_models;
	static std::vector<const char*> m_namesCstr;
	static int m_selectedModelIndex;

	static Light* s_pLight;
};

