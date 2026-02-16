#include "Button.h"

namespace Core {


	class Card {

	public:
		Card(std::unique_ptr<Button> button, Texture2D scenePreview, const char* previewText) : m_Button(std::move(button)), m_ScenePreview(scenePreview), m_PreviewText(previewText) {};
		~Card() {
		
		};

		std::unique_ptr<Button>& GetButton() { return m_Button; };
		
		Texture2D& GetScenePreview() { return m_ScenePreview; };

		const char* GetPreviewText() { return m_PreviewText; };

		void DrawCard() {

			DrawTextureRec(m_Button->GetTexture(), m_Button->GetSourceRect(), { m_Position.x, m_Position.y + m_Button->GetTexture().height}, WHITE);
			DrawTextureRec(m_ScenePreview, { 0, 0, (float)m_ScenePreview.width, (float)m_ScenePreview.height }, {m_Position.x, m_Position.y - m_ScenePreview.height}, WHITE);
			DrawText(m_PreviewText, m_Position.x + (m_Button->GetTexture().width / 2 - MeasureText(m_PreviewText, FONT_SIZE) / 2), m_Position.y - FONT_SIZE / 4, FONT_SIZE, BLACK);
			DrawText(m_Button->GetText(), m_Position.x + (m_Button->GetTexture().width / 2 - MeasureText(m_Button->GetText(), FONT_SIZE) / 2), m_Position.y + m_Button->GetTexture().height + FONT_SIZE/2, FONT_SIZE, WHITE);

		}

		void SetPosition(Vector2& pos) {
		
			m_Position = { pos.x - m_Button->GetTexture().width / 2, pos.y };
		
		};

	private:

		Vector2 m_Position = {0, 0};
		std::unique_ptr<Button> m_Button;
		Texture2D m_ScenePreview;
		const char* m_PreviewText;

	};

}
