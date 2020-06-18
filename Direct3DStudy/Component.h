class Component
{
public:
	virtual void Awake(){}
	virtual void Start(){}
	virtual void OnEnable(bool isActive){}
	virtual void Update(){}
	virtual void Destory(){}
	void aaa(){}
};

class aa : Component
{
public:
	void Awake() override{}
};