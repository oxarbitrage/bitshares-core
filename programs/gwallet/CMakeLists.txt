add_executable( gwallet
		main.cpp gwallet.cpp bitshares.cpp welcome.cpp gui_create.cpp
		dialogs/importkey.cpp
		dialogs/transfer.cpp
		modes/home.cpp
		modes/wallet.cpp
		modes/cli.cpp
		modes/sendreceive.cpp
		modes/history.cpp)

find_package(wxWidgets COMPONENTS core base adv REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries( gwallet
	PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_brief graphene_utilities graphene_wallet fc ${wxWidgets_LIBRARIES} ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

