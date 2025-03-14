#include "ThrashTheCacheComponent.h"
#include <algorithm>
#include <iostream>

//namespace dae
//{
//    ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject* owner)
//        : Component(owner), m_numberSamplesOne(10) , m_numberSamplesTwo(100)
//    {
//        m_stepSizes = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
//    }
//
//    void ThrashTheCacheComponent::Render() const
//    {
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL2_NewFrame();
//        ImGui::NewFrame();
//    
//        ImGui::Begin("Exercise 1");
//        
//        ImGui::Text("%d", m_numberSamplesOne);
//    
//        ImGui::SameLine();
//        if (ImGui::Button("-"))
//        {
//            m_numberSamplesOne = std::max(1, m_numberSamplesOne - 1);
//        }
//    
//        ImGui::SameLine();
//        if (ImGui::Button("+"))
//        {
//            m_numberSamplesOne++;
//        }
//    
//        ImGui::SameLine();
//        ImGui::Text("# samples");
//    
//        if (ImGui::Button("Trash the cache"))
//        {
//            if (!m_isRunningExercise1 && !m_isRunningExercise2)
//            {
//                m_isRunningExercise1 = true;
//                m_futureExercise1 = std::async(std::launch::async, [this]() {
//                    TrashTheCache();
//                    m_isRunningExercise1 = false;
//                    });
//            }
//        }
//    
//        if (m_isRunningExercise1)
//        {
//            ImGui::Text("Wait for it...");
//        }
//    
//        if (!m_timingsExercise1.empty())
//        {
//            ImGui::PlotConfig conf;
//            conf.values.xs = m_stepSizes.data();
//            conf.values.ys = m_timingsExercise1.data();
//            conf.values.count = static_cast<int>(m_timingsExercise1.size());
//            conf.scale.min = 0;
//            conf.scale.max = *std::max_element(m_timingsExercise1.begin(), m_timingsExercise1.end()) > 0 ?
//                *std::max_element(m_timingsExercise1.begin(), m_timingsExercise1.end()) : 1.0f;
//            conf.tooltip.show = true;
//            conf.tooltip.format = "Step: %.0f, Time: %.2f";
//            conf.grid_x.show = true;
//            conf.grid_y.show = true;
//            conf.frame_size = ImVec2(400, 200);
//            conf.line_thickness = 2.0f;
//    
//            ImGui::Plot("Trash the cache##Graph1", conf);
//        }
//    
//        ImGui::End();
//    
//        ImGui::SetNextWindowPos(ImVec2(400, 100), ImGuiCond_FirstUseEver);
//        ImGui::Begin("Exercise 2");
//    
//        ImGui::Text("%d", m_numberSamplesTwo);
//    
//        ImGui::SameLine();
//        if (ImGui::Button("-##DecreaseSamples2"))
//        {
//            m_numberSamplesTwo = std::max(1, m_numberSamplesTwo - 1);
//        }
//    
//        ImGui::SameLine();
//        if (ImGui::Button("+##IncreaseSamples2"))
//        {
//            m_numberSamplesTwo++;
//        }
//    
//        ImGui::SameLine();
//        ImGui::Text("# samples");
//    
//        if (ImGui::Button("Trash the cache with GameObject3D"))
//        {
//            if (!m_isRunningExercise1 && !m_isRunningExercise2)
//            {
//                m_isRunningExercise2 = true;
//                m_futureExercise2 = std::async(std::launch::async, [this]() {
//                    TrashTheCacheWithGameObject3D();
//                    m_isRunningExercise2 = false;
//                    });
//            }
//        }
//    
//        if (ImGui::Button("Trash the cache with GameObject3DAlt"))
//        {
//            if (!m_isRunningExercise1 && !m_isRunningExercise2)
//            {
//                m_isRunningExercise2 = true;
//                m_futureExercise2 = std::async(std::launch::async, [this]() {
//                    TrashTheCacheWithGameObject3DAlt();
//                    m_isRunningExercise2 = false;
//                    });
//            }
//        }
//    
//        if (m_isRunningExercise2)
//        {
//            ImGui::Text("Wait for it...");
//        }
//    
//        if (!m_timingsExercise2.empty())
//        {
//            ImGui::PlotConfig conf;
//            conf.values.xs = m_stepSizes.data();
//            conf.values.ys = m_timingsExercise2.data();
//            conf.values.count = static_cast<int>(m_timingsExercise2.size());
//            conf.scale.min = 0;
//            conf.scale.max = *std::max_element(m_timingsExercise2.begin(), m_timingsExercise2.end()) > 0 ?
//                *std::max_element(m_timingsExercise2.begin(), m_timingsExercise2.end()) : 1.0f;
//            conf.tooltip.show = true;
//            conf.tooltip.format = "Step: %.0f, Time: %.2f";
//            conf.grid_x.show = true;
//            conf.grid_y.show = true;
//            conf.frame_size = ImVec2(400, 200);
//            conf.line_thickness = 2.0f;
//    
//            ImGui::Plot("GameObject3D##Graph2", conf); 
//        }
//    
//        if (!m_timingsExercise2Alt.empty())
//        {
//            ImGui::PlotConfig conf;
//            conf.values.xs = m_stepSizes.data();
//            conf.values.ys = m_timingsExercise2Alt.data();
//            conf.values.count = static_cast<int>(m_timingsExercise2Alt.size());
//            conf.scale.min = 0;
//            conf.scale.max = *std::max_element(m_timingsExercise2Alt.begin(), m_timingsExercise2Alt.end()) > 0 ?
//                *std::max_element(m_timingsExercise2Alt.begin(), m_timingsExercise2Alt.end()) : 1.0f;
//            conf.tooltip.show = true;
//            conf.tooltip.format = "Step: %.0f, Time: %.2f";
//            conf.grid_x.show = true;
//            conf.grid_y.show = true;
//            conf.frame_size = ImVec2(400, 200);
//            conf.line_thickness = 2.0f;
//    
//            ImGui::Plot("GameObject3DAlt##Graph2Alt", conf); 
//        }
//
//        if (!m_timingsExercise2.empty() && !m_timingsExercise2Alt.empty())
//        {
//            ImGui::Text("Combined");
//
//            ImGui::PlotConfig conf;
//            conf.values.xs = m_stepSizes.data(); 
//            conf.values.count = static_cast<int>(m_timingsExercise2.size()); 
//
//            conf.values.ys_list = new const float* [2]; 
//            conf.values.ys_list[0] = m_timingsExercise2.data();
//            conf.values.ys_list[1] = m_timingsExercise2Alt.data(); 
//            conf.values.ys_count = 2; 
//
//            conf.scale.min = 0;
//            conf.scale.max = std::max(
//                *std::max_element(m_timingsExercise2.begin(), m_timingsExercise2.end()),
//                *std::max_element(m_timingsExercise2Alt.begin(), m_timingsExercise2Alt.end())
//            ) > 0 ? std::max(
//                *std::max_element(m_timingsExercise2.begin(), m_timingsExercise2.end()),
//                *std::max_element(m_timingsExercise2Alt.begin(), m_timingsExercise2Alt.end())
//            ) : 1.0f;
//            conf.tooltip.show = true;
//            conf.tooltip.format = "Step: %.0f, Time: %.2f";
//            conf.grid_x.show = true;
//            conf.grid_y.show = true;
//            conf.frame_size = ImVec2(400, 200);
//            conf.line_thickness = 2.0f;
//
//            ImGui::Plot("Combined##Graph2Combined", conf);
//
//            delete[] conf.values.ys_list;
//        }
//
//        ImGui::End();
//
//        ImGui::Render(); 
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
//    }
//
//    void ThrashTheCacheComponent::TrashTheCache() const
//    {
//        const size_t bufferSize = 1 << 26;
//        std::vector<int> buffer(bufferSize, 1);
//
//        m_timingsExercise1.clear();
//
//        for (size_t stepSize = 1; stepSize <= 1024; stepSize *= 2)
//        {
//            const auto start = std::chrono::high_resolution_clock::now();
//
//            for (size_t i = 0; i < bufferSize; i += stepSize)
//            {
//                buffer[i] *= 2;
//            }
//
//            const auto end = std::chrono::high_resolution_clock::now();
//            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//            m_timingsExercise1.push_back(static_cast<float>(duration));
//        }
//
//        std::cout << "Exercise 1 - Step Size,Timing (microseconds)\n";
//        for (size_t stepSize = 1, i = 0; stepSize <= 1024; stepSize *= 2, ++i)
//        {
//            std::cout << stepSize << "," << m_timingsExercise1[i] << "\n";
//        }
//    }
//
//    void ThrashTheCacheComponent::TrashTheCacheWithGameObject3D() const
//    {
//        const size_t bufferSize = 1 << 26; 
//        std::vector<GameObject3D> buffer(bufferSize);
//
//        m_timingsExercise2.clear();
//
//        for (size_t stepSize = 1; stepSize <= 1024; stepSize *= 2)
//        {
//            const auto start = std::chrono::high_resolution_clock::now();
//
//            for (size_t i = 0; i < bufferSize; i += stepSize)
//            {
//                buffer[i].ID *= 2;
//            }
//
//            const auto end = std::chrono::high_resolution_clock::now();
//            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//            m_timingsExercise2.push_back(static_cast<float>(duration));
//        }
//
//        std::cout << "Exercise 2 - Step Size,Timing (microseconds)\n";
//        for (size_t stepSize = 1, i = 0; stepSize <= 1024; stepSize *= 2, ++i)
//        {
//            std::cout << stepSize << "," << m_timingsExercise2[i] << "\n";
//        }
//    }
//
//    void ThrashTheCacheComponent::TrashTheCacheWithGameObject3DAlt() const
//    {
//        const size_t bufferSize = 1 << 26;
//        GameObject3DAltContainer buffer;
//        buffer.IDs.resize(bufferSize);
//        buffer.transforms.resize(bufferSize);
//
//        m_timingsExercise2Alt.clear(); 
//
//        for (size_t stepSize = 1; stepSize <= 1024; stepSize *= 2)
//        {
//            const auto start = std::chrono::high_resolution_clock::now();
//
//            for (size_t i = 0; i < bufferSize; i += stepSize)
//            {
//                buffer.IDs[i] *= 2;
//            }
//
//            const auto end = std::chrono::high_resolution_clock::now();
//            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//
//            m_timingsExercise2Alt.push_back(static_cast<float>(duration)); 
//        }
//
//        std::cout << "Exercise 2 (Alt) - Step Size,Timing (microseconds)\n";
//        for (size_t stepSize = 1, i = 0; stepSize <= 1024; stepSize *= 2, ++i)
//        {
//            std::cout << stepSize << "," << m_timingsExercise2Alt[i] << "\n";
//        }
//    }
//}

