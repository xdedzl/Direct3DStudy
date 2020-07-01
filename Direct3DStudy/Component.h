namespace XEngine
{
	class Component
	{
	public:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable(bool isActive) {}
		virtual void Update() {}
		virtual void Destory() {}
	};

	class Transform : Component
	{
	public:
		void Awake() override {}
	};

	class Render : Component
	{

	};

	class Light :Component
	{

	};

	class Camera : Component
	{

	};
}