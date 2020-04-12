//Abraham Villatoro Tello  BullCow Game - Unreal Engine Course Ver-2019.
#include "BullCowCartridge.h"
#include "HiddenWordList.h" 
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame();    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }  
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull Cows game!"));
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives\n"), Lives);
    PrintLine(TEXT("Type in your guess and \nPress enter to continue..."));
    //PrintLine(FString::Printf(TEXT("The Hidden word is: %s"), *HiddenWord));//Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) 
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letter long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry try guessing again, \nYou have %i lives remaining"), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters. guess again"));
        return;
    }

        PrintLine(TEXT("Lost a life!"));
        --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();       
    }

    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess agan, you have %i lives left"), Lives);
    
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word [Comparison])
            {
                return false;
            }   
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls ++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows ++;
                break;
            }
        }
    }
    return Count;
}