#include "Scene.h"
#include "../Mesh/Model.h"
#include "../Mesh/Mesh.h"
#include "../Dependencies/assimp/include/assimp/Importer.hpp";
#include "../Dependencies/assimp/include/assimp/scene.h";
#include "../Dependencies/assimp/include/assimp/postprocess.h";
#include "../Dependencies/assimp/include/assimp/Exporter.hpp";
#include <fstream>
#include "../AppWindow/AppWindow.h"
//#include "../Dependencies/assimp/include/assimp/ColladaExporter.h";

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
void Scene::XORShifrate(char* source, int size)  noexcept{
    char key[] = "sMESHer";
    for (int i = 0; i < size; i++) {
        source[i] = source[i] ^ key[i % 8];
    }
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
void Scene::SaveAll() noexcept {
    if (m_models.empty()) return;

    aiScene* scene = new aiScene();

    scene->mRootNode = new aiNode();

    scene->mMaterials = new aiMaterial * [1];
    scene->mMaterials[0] = nullptr;
    scene->mNumMaterials = 1;

    scene->mMaterials[0] = new aiMaterial();

    scene->mMeshes = new aiMesh * [m_models.size()];
    //scene.mMeshes[0] = nullptr;
    scene->mNumMeshes = m_models.size();


    scene->mRootNode->mMeshes = new unsigned int[m_models.size()];
    for (int i = 0; i < m_models.size(); i++) {
        scene->mRootNode->mMeshes[i] = i;
    }
    scene->mRootNode->mNumMeshes = m_models.size();


    for (int m = 0; m < m_models.size(); m++) {
        scene->mMeshes[m] = new aiMesh();
        scene->mMeshes[m]->mMaterialIndex = 0;
        scene->mMeshes[m]->mNumVertices = m_models[m]->m_pMeshe->m_sourceVerticiesNum;
        scene->mMeshes[m]->mVertices = new aiVector3D[scene->mMeshes[m]->mNumVertices];

        for (int i = 0; i < m_models[m]->m_pMeshe->m_sourceVerticiesNum; i++) {
            scene->mMeshes[m]->mVertices[i] = aiVector3D(m_models[m]->m_pMeshe->m_pSourceVerticies[i].position.x, m_models[m]->m_pMeshe->m_pSourceVerticies[i].position.y, m_models[m]->m_pMeshe->m_pSourceVerticies[i].position.z);
        }

        scene->mMeshes[m]->mNumFaces = m_models[m]->m_pMeshe->m_sourceIndeciesNum;
        scene->mMeshes[m]->mFaces = new aiFace[scene->mMeshes[m]->mNumFaces];

        for (int i = 0; i < m_models[m]->m_pMeshe->m_sourceIndeciesNum; i++) {
            aiFace& face = scene->mMeshes[m]->mFaces[i];

            face.mIndices = new unsigned int[3];
            face.mNumIndices = 3;

            face.mIndices[0] = m_models[m]->m_pMeshe->m_pSourceIndecies[i].I1;
            face.mIndices[1] = m_models[m]->m_pMeshe->m_pSourceIndecies[i].I2;
            face.mIndices[2] = m_models[m]->m_pMeshe->m_pSourceIndecies[i].I3;
        }
       
    }

    OPENFILENAMEA saveFileDialog;
    char szSaveFileName[MAX_PATH] = "Untilted";
    ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
    saveFileDialog.lStructSize = sizeof(saveFileDialog);
    saveFileDialog.hwndOwner = AppWindow::s_hWnd;
    saveFileDialog.lpstrFilter = "Model (*.dae)";
    saveFileDialog.lpstrFile = szSaveFileName;
    saveFileDialog.nMaxFile = MAX_PATH;
    saveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    saveFileDialog.lpstrDefExt = "dae";

    if (GetSaveFileNameA(&saveFileDialog)) {
        Assimp::Exporter exporter;
        const aiExportFormatDesc* format = exporter.GetExportFormatDescription(0);
        exporter.Export(scene, "collada", saveFileDialog.lpstrFile);
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

void Scene::ShifrateFile() noexcept{
    OPENFILENAMEA ofn = { 0 };
    char* szFile = new char[260]();
    char* szFileTitle = new char[260]();
    // Initialize remaining fields of OPENFILENAME structure
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = AppWindow::s_hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 260 * 2;
    ofn.lpstrFilter = "Model\0*.DAE;*.FBX;*.OBJ\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 260 * 2;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


    char* a = nullptr;
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        //Display the selected file. 
        ofn.lpstrFile;
        std::ifstream ifs(ofn.lpstrFile, std::ios::ate | std::ios::binary);
        int size = ifs.tellg();
        ifs.seekg(0);
        char* buf = new char[size];
        ifs.read(buf, size);
        ifs.close();

        XORShifrate(buf, size);

        std::ofstream ofs(ofn.lpstrFile, std::ios::binary);
        ofs.write(buf, size);
        ofs.close();
        delete[] buf;
    }
}