// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts.
{
    Super::BeginPlay();
	Isograms = GetValidWords(WordList);
	SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	FString LowerCaseInput = Input.ToLower();
	if (bGameOver)
	{
		ClearScreen();
		SetupGame();
	}
	else
	{
		CheckPlayerGuess(LowerCaseInput);
	}
	// Remove a life
}

void	UBullCowCartridge::SetupGame()
{
	PrintLine(TEXT("Hi, welcome to the Bull Cow game!\nYou can use the terminal by pressing tab."));

	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
	Lives = HiddenWord.Len() * 2;
	bGameOver = false;
	PrintLine(TEXT("Guess a %i letter word that is an isogram.\nYou have %i lives, you'll lose a life\nfor every wrong guess.\nYou get one Bull for a right letter in\nin the right place and one Cow\nfor every right letter in the wrong place."), HiddenWord.Len(), (HiddenWord.Len() * 2));
	// PrintLine(TEXT("The hidden word is: %s."), *HiddenWord); // Debug line, REMOVE!
}

void	UBullCowCartridge::EndGame()
{
	bGameOver = true;
	PrintLine(TEXT("Press 'enter' to play again."));
}

void	UBullCowCartridge::CheckPlayerGuess(const FString& Guess)
{
	int32 WordLen = HiddenWord.Len();
	if (Guess == HiddenWord)
	{
		ClearScreen();
		PrintLine(Guess);
		PrintLine(TEXT("Correct!!"));
		EndGame();
		return;
	}
	if (Guess.Len() != WordLen)
	{
		ClearScreen();
		PrintLine(Guess);
		PrintLine(TEXT("The Hidden Word is %i characters long.\nYou have %i lives remaining.\nTry another word."), WordLen, Lives);
		return;
	}
 	if(!IsIsogram(Guess))
	{
		ClearScreen();
		PrintLine(Guess);
		PrintLine(TEXT("Your word has repeating letters,\nan isogram doesn't.\nGuess again."));
		return;
	}
	--Lives;
	if (Lives < 1)
	{
		ClearScreen();
		PrintLine(TEXT("Game over, you ran out of lives.\n"));
		PrintLine(TEXT("The hidden word was: %s.\n"), *HiddenWord);
		EndGame();
		return;
	}
	ClearScreen();
	PrintLine(Guess);

	// Show the player Bulls and Cows.
	FBullCowCount Count = {0, 0};
	Count = GetBullCows(Guess, Count);
	PrintLine(TEXT("You have %i Bulls and %i Cows."), Count.Bulls, Count.Cows);
	PrintLine(TEXT("Wrong.. You have %i lives remaining.\nTry another word."), Lives);
}

bool	UBullCowCartridge::IsIsogram(const FString& Word) const
{
	for (int32 Index = 0; Index < Word.Len(); Index++)
	{
		for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
		{
			if (Word[Index] == Word[Comparison]) // Loop through the word, compare the letters
				return false;
		}
	}
	return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
	TArray<FString> ValidWords;
	for (FString TempWord : WordList)
	{
		if (TempWord.Len() >= 4 && TempWord.Len() <= 8 && IsIsogram(TempWord))
		{
			ValidWords.Emplace(TempWord);
		}
	}
	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess, FBullCowCount& Count) const
{
	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}
		for (int32 CowIndex = GuessIndex; CowIndex < HiddenWord.Len(); CowIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[CowIndex])
			{
				Count.Cows++;
				break;
			}
		}
		
	}
	return Count;
	
}