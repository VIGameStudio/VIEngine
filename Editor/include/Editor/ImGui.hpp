#pragma once

namespace vi
{
	class ImGui
	{
	public:
		static bool Initialize(const void* pWindow);
		static void Shutdown();

		static void NewFrame();
		static void Render();

		static bool Begin(const char* name);
		static void End();

        //static void PushItemWidth(float item_width);
        //static void PopItemWidth();
        //static void SetNextItemWidth(float item_width);
        //static float CalcItemWidth();
        //static void PushTextWrapPos(float wrap_local_pos_x = 0.0f);
        //static void PopTextWrapPos();
        
        //static ImFont* GetFont();
        //static float GetFontSize();
        //static ImVec2 GetFontTexUvWhitePixel();
        //static ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);
        //static ImU32 GetColorU32(const ImVec4& col);
        //static ImU32 GetColorU32(ImU32 col);
        //static const ImVec4& GetStyleColorVec4(ImGuiCol idx);
        
        //static void Separator();
        //static void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
        //static void NewLine();
        //static void Spacing();
        //static void Dummy(const ImVec2& size);
        //static void Indent(float indent_w = 0.0f);
        //static void Unindent(float indent_w = 0.0f);
        //static void BeginGroup();
        //static void EndGroup();
        //static ImVec2 GetCursorPos();
        //static float GetCursorPosX();
        //static float GetCursorPosY();
        //static void SetCursorPos(const ImVec2& local_pos);
        //static void SetCursorPosX(float local_x);
        //static void SetCursorPosY(float local_y);
        //static ImVec2 GetCursorStartPos();
        //static ImVec2 GetCursorScreenPos();
        //static void SetCursorScreenPos(const ImVec2& pos);
        //static void AlignTextToFramePadding();
        //static float GetTextLineHeight();
        //static float GetTextLineHeightWithSpacing();
        //static float GetFrameHeight();
        //static float GetFrameHeightWithSpacing();
        
        //static void PushID(const char* str_id);
        //static void PushID(const char* str_id_begin, const char* str_id_end);
        //static void PushID(const void* ptr_id);
        //static void PushID(int int_id);
        //static void PopID();
        //static ImGuiID GetID(const char* str_id);
        //static ImGuiID GetID(const char* str_id_begin, const char* str_id_end);
        //static ImGuiID GetID(const void* ptr_id);
        
        //static void TextUnformatted(const char* text, const char* text_end = NULL);
        //static void Text(const char* fmt, ...);
        //static void TextV(const char* fmt, va_list args);
        //static void TextColored(const ImVec4& col, const char* fmt, ...);
        //static void TextColoredV(const ImVec4& col, const char* fmt, va_list args);
        //static void TextDisabled(const char* fmt, ...);
        //static void TextDisabledV(const char* fmt, va_list args);
        //static void TextWrapped(const char* fmt, ...);
        //static void TextWrappedV(const char* fmt, va_list args);
        //static void LabelText(const char* label, const char* fmt, ...);
        //static void LabelTextV(const char* label, const char* fmt, va_list args);
        //static void BulletText(const char* fmt, ...);
        //static void BulletTextV(const char* fmt, va_list args);
        //static void SeparatorText(const char* label);
        
        static bool Button(const char* label, float w, float h);
        //static bool SmallButton(const char* label);
        //static bool InvisibleButton(const char* str_id, const ImVec2& size, ImGuiButtonFlags flags = 0);
        //static bool ArrowButton(const char* str_id, ImGuiDir dir);
        //static bool Checkbox(const char* label, bool* v);
        //static bool CheckboxFlags(const char* label, int* flags, int flags_value);
        //static bool CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
        //static bool RadioButton(const char* label, bool active);
        //static bool RadioButton(const char* label, int* v, int v_button);
        //static void ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-FLT_MIN, 0), const char* overlay = NULL);
        //static void Bullet();
        
        //static void Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
        //static bool ImageButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
        
        //static bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
        //static void EndCombo();
        //static bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
        //static bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
        //static bool Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
        
        static float DragFloat(const char* label, float v);// , float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
        //static bool DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
        //static bool DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);
        //static bool DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);

        static float SliderFloat(const char* label, float v, float v_min, float v_max);// , const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0);
        //static bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
        //static bool SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
        //static bool VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
        //static bool VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
        //static bool VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
        
        //static bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        //static bool InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        //static bool InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        //static bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
        //static bool InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
        //static bool InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
        //static bool InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
        //static bool InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
        //static bool InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
        //static bool InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
        //static bool InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
        //static bool InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
        //static bool InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
        //static bool InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
        
        //static bool ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
        //static bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
        //static bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
        //static bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
        //static bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
        //static void SetColorEditOptions(ImGuiColorEditFlags flags);
        
        //static bool TreeNode(const char* label);
        //static bool TreeNode(const char* str_id, const char* fmt, ...);
        //static bool TreeNode(const void* ptr_id, const char* fmt, ...);
        //static bool TreeNodeV(const char* str_id, const char* fmt, va_list args);
        //static bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args);
        //static bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
        //static bool TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...);
        //static bool TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...);
        //static bool TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
        //static bool TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
        //static void TreePush(const char* str_id);
        //static void TreePush(const void* ptr_id);
        //static void TreePop();
        //static float GetTreeNodeToLabelSpacing();
        //static bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);
        //static bool CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0);
        //static void SetNextItemOpen(bool is_open, ImGuiCond cond = 0);
        
        //static bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
        //static bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
        
        //static bool BeginListBox(const char* label, const ImVec2& size = ImVec2(0, 0));
        //static void EndListBox();
        //static bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
        //static bool ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
        
        //static void PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
        //static void PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
        //static void PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
        //static void PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
        
        //static void Value(const char* prefix, bool b);
        //static void Value(const char* prefix, int v);
        //static void Value(const char* prefix, unsigned int v);
        //static void Value(const char* prefix, float v, const char* float_format = NULL);
        
        static bool BeginMenuBar();
        static void EndMenuBar();
        static bool BeginMainMenuBar();
        static void EndMainMenuBar();
        static bool BeginMenu(const char* label);// , bool enabled = true);
        static void EndMenu();
        static bool MenuItem(const char* label);//, const char* shortcut = NULL, bool selected = false, bool enabled = true);
        //static bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);
        
        //static void BeginTooltip();
        //static void EndTooltip();
        //static void SetTooltip(const char* fmt, ...);
        //static void SetTooltipV(const char* fmt, va_list args);
        
        //static bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
        //static bool BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
        //static void EndPopup();
        
        //static void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);
        //static void OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);
        //static void OpenPopupOnItemClick(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);
        //static void CloseCurrentPopup();
        
        //static bool BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);
        //static bool BeginPopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);
        //static bool BeginPopupContextVoid(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);
        
        //static bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0);
        
        //static bool BeginTable(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);
        //static void EndTable();
        //static void TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f);
        //static bool TableNextColumn();
        //static bool TableSetColumnIndex(int column_n);
        
        //static void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
        //static void TableSetupScrollFreeze(int cols, int rows);
        //static void TableHeadersRow();
        //static void TableHeader(const char* label);
        
        //static ImGuiTableSortSpecs* TableGetSortSpecs();
        //static int TableGetColumnCount();
        //static int TableGetColumnIndex();
        //static int TableGetRowIndex();
        //static const char* TableGetColumnName(int column_n = -1);
        //static ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);
        //static void TableSetColumnEnabled(int column_n, bool v);
        //static void TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1);
        
        //static void Columns(int count = 1, const char* id = NULL, bool border = true);
        //static void NextColumn();
        //static int GetColumnIndex();
        //static float GetColumnWidth(int column_index = -1);
        //static void SetColumnWidth(int column_index, float width);
        //static float GetColumnOffset(int column_index = -1);
        //static void SetColumnOffset(int column_index, float offset_x);
        //static int GetColumnsCount();
        
        //static bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);
        //static void EndTabBar();
        //static bool BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);
        //static void EndTabItem();
        //static bool TabItemButton(const char* label, ImGuiTabItemFlags flags = 0);
        //static void SetTabItemClosed(const char* tab_or_docked_window_label);

		static void ShowDemoWindow();
	};
}