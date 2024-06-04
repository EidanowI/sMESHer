#include "Scene.h"

std::vector<Model*> Scene::m_models = std::vector<Model*>();
std::vector<const char*> Scene::m_namesCstr = std::vector<const char*>();
int Scene::m_selectedModelIndex = 0;
Light* Scene::s_pLight = nullptr;



void Scene::QsortRecursive(Model** arr, int size) {
    //Указатели в начало и в конец массива
    int i = 0;
    int j = size - 1;

    //Центральный элемент массива
    Model* mid = arr[size / 2];

    //Делим массив
    do {
        //Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        //В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
        while (arr[i]->m_nameLength < mid->m_nameLength) {
            i++;
        }
        //В правой части пропускаем элементы, которые больше центрального
        while (arr[j]->m_nameLength > mid->m_nameLength) {
            j--;
        }

        //Меняем элементы местами
        if (i <= j) {
            Model* tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);


    //Рекурсивные вызовы, если осталось, что сортировать
    if (j > 0) {
        //"Левый кусок"
        QsortRecursive(arr, j + 1);
    }
    if (i < size) {
        //"Првый кусок"
        QsortRecursive(&arr[i], size - i);
    }
}
int Scene::InterpolateSearch(int searchLength) noexcept {
    int midle = 0;
    int lowIndex = 0;
    int highIndex = m_models.size() - 1;

    while (m_models[lowIndex]->m_nameLength <= searchLength && m_models[highIndex]->m_nameLength >= searchLength)
    {
        midle = lowIndex 
            + ((searchLength - m_models[lowIndex]->m_nameLength) 
                * (highIndex - lowIndex)) 
            / (m_models[highIndex]->m_nameLength - m_models[lowIndex]->m_nameLength);
        if (m_models[midle]->m_nameLength < searchLength) lowIndex = midle + 1;
        else if (m_models[midle]->m_nameLength > searchLength) highIndex = midle - 1;
        else return midle;
    }

    if (m_models[lowIndex]->m_nameLength == searchLength) return lowIndex;
    else return -1;
}

void Scene::Render() noexcept {
    for (int i = 0; i < m_models.size(); i++) {
        m_models[i]->Draw();
    }
}

void Scene::Clear() noexcept {
    m_models.clear();
}
void Scene::RecreateNames() noexcept {
	m_selectedModelIndex = 0;

	m_namesCstr.clear();

	for (int i = 0; i < m_models.size(); i++) {
		m_namesCstr.push_back(m_models[i]->m_name);
	}
}

void Scene::SortModels() noexcept {
	//гарантирует что индекс выбранной в массиве модели будет указывать
	//на ту же модель даже после смены её места в массиве
    if (m_models.empty()) return;
    Model* selectedModel = m_models[m_selectedModelIndex];



    Model** modelArr = new Model * [m_models.size()];
    int modelSize = m_models.size();
    for (int i = 0; i < m_models.size(); i++) {
        modelArr[i] = m_models[i];
    }
    QsortRecursive(modelArr, m_models.size());

    m_models.clear();
    for (int i = 0; i < modelSize; i++) {
        m_models.push_back(modelArr[i]);
        if (modelArr[i] == selectedModel) {
            m_selectedModelIndex = i;
        }
    }

    delete[] modelArr;

	m_namesCstr.clear();

	for (int i = 0; i < m_models.size(); i++) {
		m_namesCstr.push_back(m_models[i]->m_name);
	}
}
void Scene::BinarySearchModels(char* searchName, int length) noexcept{
    int res = InterpolateSearch(length);
    //int res = 0;
    if (res == -1) return;

    while (res > 1 && m_models[res]->m_nameLength == length) {
        res--;
    }
    while (res < m_models.size() && m_models[res]->m_nameLength == length) {
        bool isIt = true;
        for (int i = 0; i < length; i++) {
            if (m_models[res]->m_name[i] != searchName[i]) isIt = false;
        }

        if (isIt) {
            m_selectedModelIndex = res;
            return;
        }

        res++;
    }
}

void Scene::UpdateLight() noexcept {
    s_pLight->Update();
}
void Scene::InitLight() noexcept {
    s_pLight = new Light();
}
void Scene::TerminateLight() noexcept {
    delete s_pLight;
}