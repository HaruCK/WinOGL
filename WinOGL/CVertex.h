#pragma once

class CVertex
{
public:
	CVertex(); 
	~CVertex();
	CVertex(float new_x, float new_y);
	CVertex(float new_x, float new_y, CVertex* new_next);

private:
	//�@���_��X���W
	float x;
	//�@���_��Y���W
	float y;
	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex;

public:
	//�@���_��X���W����������
	void SetX(float new_x);
	//�@���_��Y���W����������
	void SetY(float new_y);
	//�@���_��X�EY���W�̗�������������
	void SetXY(float new_x, float new_y);

	//�@���_��X���W��ǂݍ���
	float GetX();
	//�@���_��Y���W��ǂݍ���
	float GetY();
	//�@���_��X�EY���W�̗�����ǂݍ���
	void GetXY(float* get_x, float* get_y);

	//�@���̒��_���X�g���w���|�C���^����������
	void SetNext(CVertex* new_next);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();

	void FreeVertex();
};

