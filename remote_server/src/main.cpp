int main() {
	std::cout << "boost works:" << boost::filesystem::current_path().string() << std::endl;
	std::cout << "poco works:" << Poco::LocalDateTime().day() << std::endl;
	std::cout << "wxwidgets works:" << wxString::Format(wxT("%i"), 999) << std::endl;
}