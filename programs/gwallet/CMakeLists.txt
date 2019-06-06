add_executable( gwallet
		main.cpp gwallet.cpp bitshares.cpp

		dialogs/importkey.cpp

		panels/transfer.cpp
        panels/sellasset.cpp
        panels/borrowasset.cpp
        panels/cancelorder.cpp
        panels/setproxy.cpp
        panels/getcommitteemember.cpp
		panels/getaccounthistory.cpp
		panels/getorderbook.cpp
		panels/info.cpp
        panels/wallet.cpp
        panels/cli.cpp
        panels/sendreceive.cpp
        panels/dashboard.cpp
		panels/commands.cpp

		panels/about.cpp
		panels/active_witnesses.cpp
		panels/active_committee.cpp

		wizards/welcome.cpp
		)

find_package(wxWidgets COMPONENTS core base adv xrc aui REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries( gwallet
	PRIVATE graphene_app graphene_net graphene_chain graphene_egenesis_brief graphene_utilities graphene_wallet fc
		${wxWidgets_LIBRARIES} ${CMAKE_DL_LIBS} ${PLATFORM_SPECIFIC_LIBS} )

