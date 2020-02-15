USE [master]
GO

IF NOT EXISTS (SELECT * FROM sys.databases WHERE NAME = 'PhoneBook')
BEGIN
	CREATE DATABASE [PhoneBook] ON
	( 
		NAME = 'PhoneBook',
		FILENAME = 'C:\Program Files\Microsoft SQL Server\MSSQL13.MSSQLSERVER\MSSQL\DATA\PhoneBook.mdf',
		SIZE = 512KB,
		MAXSIZE = 1024KB,
		FILEGROWTH = 16KB
	)
	 LOG ON 
	(
		NAME = N'PhoneBook_log',
		FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL13.MSSQLSERVER\MSSQL\DATA\PhoneBook_log.ldf',
		SIZE = 1024KB,
		MAXSIZE = 2048KB,
		FILEGROWTH = 32KB
	)
END