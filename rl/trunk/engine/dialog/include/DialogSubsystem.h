#ifndef __DialogSubsystem_H__
#define __DialogSubsystem_H__

#include "OgreSingleton.h"
#include "DialogPrerequisites.h"
#include "OgreSingleton.h"

namespace rl
{
	class _RlDialogExport DialogSubsystem: protected Singleton<DialogSubsystem>
	{
	public:
		DialogSubsystem();
		virtual ~DialogSubsystem();

		static DialogSubsystem & getSingleton(void);
        static DialogSubsystem * getSingletonPtr(void);
	};
}
#endif;
