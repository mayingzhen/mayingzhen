#ifndef TEXT_H
#define TEXT_H


#include <d3dx9.h>
#include <d3d9.h>

class Text
{
private:
	char* m_s;
	int m_size;
	D3DCOLOR m_color;
	ID3DXFont* m_font;
	RECT	m_rect;
public:
	Text(char* s, int size, D3DCOLOR color);
	Text(char* s, int size);
	~Text();

	void Render();

	void SetText(char* s);
	void SetColor(D3DCOLOR color);
	void SetSize(int size);
	void SetPos(int x, int y);

	char*	 GetText()	{return m_s;}
	D3DCOLOR GetColor() {return m_color;}
	int		 GetSize()	{return m_size;}
	int		 GetPosX() {return m_rect.left;}
	int		 GetPosY() {return m_rect.top;}
};


#endif