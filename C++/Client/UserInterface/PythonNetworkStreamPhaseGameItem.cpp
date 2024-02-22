//Search

		case SE_HPUP_RED:
			effect = CInstanceBase::EFFECT_HPUP_RED;
			bPlayPotionSound = true;
			break;

//Add before


#ifdef ENABLE_SUNGMAHEE_TOWER
		case SE_EFFECT_SMH_CIRCLE_THORN:
			effect = CInstanceBase::EFFECT_SMH_CIRCLE_THORN;
			bAttachEffect = false;
			break;
#endif