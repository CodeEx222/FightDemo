


#include "GameAnimInstance.h"

float UGameAnimInstance::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName,
	EMontagePlayReturnType ReturnValueType)
{
	if(AnimMontage)
	{
		float const Duration = this->Montage_Play(AnimMontage, InPlayRate,ReturnValueType);
		if ( Duration > 0.f)
		{
			// Start at a given Section.
			if( StartSectionName != NAME_None )
			{
				this->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}
