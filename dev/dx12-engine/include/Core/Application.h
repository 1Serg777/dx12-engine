namespace dxe
{
	class Application
	{
	public:

		virtual void Initialize() = 0;
		virtual void Terminate() = 0;
		virtual void Run() = 0;
	};
}